#include "../include/RegexNormalize.h"
#include <set>
#include <algorithm>
#include <map>
#include <cassert>

/*
 * canonicalizeChildren(v)
 *
 * Utility used by UNION and CONCAT normalization.
 * Performs:
 *   - removal of null children
 *   - lexicographic ordering using each child's key()
 *   - duplicate removal
 *
 * Ensures that UNION and CONCAT expressions have a consistent,
 * canonical ordering of subexpressions.
 */
static void canonicalizeChildren(std::vector<std::shared_ptr<Regex>>& v) {
    std::vector<std::shared_ptr<Regex>> tmp;
    tmp.reserve(v.size());
    for (auto &c : v)
        if (c) tmp.push_back(c);
    v.swap(tmp);

    std::sort(v.begin(), v.end(),
        [](const std::shared_ptr<Regex> &a, const std::shared_ptr<Regex> &b){
            return a->key() < b->key();
        });

    std::vector<std::shared_ptr<Regex>> out;
    std::string last = "";
    for (auto &c : v) {
        std::string k = c->key();
        if (k != last) {
            out.push_back(c);
            last = k;
        }
    }
    v.swap(out);
}

/*
 * normalizeRegexAST(r)
 *
 * Performs structural normalization on the regex AST:
 *   - eliminates redundant EPS
 *   - collapses nested CONCAT/UNION nodes
 *   - removes EMPTYSET where allowed
 *   - simplifies (r*)* → r*
 *   - simplifies (#)* → #
 *   - canonicalizes ordering of UNION nodes
 *
 * Called repeatedly in the regex-canonicalization pipeline.
 */
std::shared_ptr<Regex> normalizeRegexAST(std::shared_ptr<Regex> r) {
    if (!r) return r;

    switch (r->kind) {

        case RKind::LITERAL:
        case RKind::EPS:
        case RKind::EMPTYSET:
            return r;

        case RKind::STAR: {
            if (!r->child) return Regex::makeEps();
            auto inner = normalizeRegexAST(r->child);
            if (!inner) return Regex::makeEps();

            /* (r*)* → r* */
            if (inner->kind == RKind::STAR) {
                if (!inner->child) return Regex::makeEps();
                return Regex::makeStar(inner->child);
            }

            /* (#)* → # */
            if (inner->kind == RKind::EPS)
                return Regex::makeEps();

            return Regex::makeStar(inner);
        }

        case RKind::CONCAT: {
            std::vector<std::shared_ptr<Regex>> items;
            items.reserve(r->children.size());

            for (auto &c : r->children) {
                if (!c) continue;
                auto n = normalizeRegexAST(c);
                if (!n) continue;

                /* φ destroys concatenation */
                if (n->kind == RKind::EMPTYSET)
                    return Regex::makeEmpty();

                /* ignore ε */
                if (n->kind == RKind::EPS)
                    continue;

                /* flatten nested concatenations */
                if (n->kind == RKind::CONCAT) {
                    for (auto &g : n->children)
                        if (g) items.push_back(g);
                }
                else {
                    items.push_back(n);
                }
            }

            if (items.empty()) return Regex::makeEps();
            if (items.size() == 1) return items[0];

            return Regex::makeConcat(items);
        }

        case RKind::UNION: {
            std::vector<std::shared_ptr<Regex>> items;
            items.reserve(r->children.size());

            for (auto &c : r->children) {
                if (!c) continue;
                auto n = normalizeRegexAST(c);
                if (!n) continue;

                /* remove φ inside union */
                if (n->kind == RKind::EMPTYSET)
                    continue;

                /* flatten nested unions */
                if (n->kind == RKind::UNION) {
                    for (auto &g : n->children)
                        if (g) items.push_back(g);
                }
                else {
                    items.push_back(n);
                }
            }

            if (items.empty()) return Regex::makeEmpty();
            canonicalizeChildren(items);
            if (items.size() == 1) return items[0];

            return Regex::makeUnion(items);
        }
    }

    return r;
}

/*
 * boundedDistribute(r)
 *
 * Attempts a small, controlled form of distributive expansion:
 *      (x | y) z → xz | yz
 *
 * Distribution is only attempted when:
 *   - The UNION has at most two alternatives, and
 *   - The resulting expression strictly reduces the heuristic cost.
 *
 * Prevents exponential growth while still enabling helpful rewrites.
 */
std::shared_ptr<Regex> boundedDistribute(std::shared_ptr<Regex> r) {
    if (!r) return r;

    /* Do not distribute inside these forms */
    if (r->kind == RKind::STAR ||
        r->kind == RKind::UNION ||
        r->kind == RKind::EPS ||
        r->kind == RKind::LITERAL ||
        r->kind == RKind::EMPTYSET)
    {
        return r;
    }

    if (r->kind == RKind::CONCAT) {
        auto childrenCopy = r->children;

        for (size_t i = 0; i < childrenCopy.size(); ++i) {
            auto child = childrenCopy[i];
            if (!child) continue;

            /* Only distribute if UNION has at most 2 items */
            if (child->kind == RKind::UNION && child->children.size() <= 2) {

                std::vector<std::shared_ptr<Regex>> alts;
                alts.reserve(child->children.size());

                for (auto &alt : child->children) {
                    if (!alt) continue;

                    std::vector<std::shared_ptr<Regex>> newItems;
                    newItems.reserve(childrenCopy.size());

                    for (size_t j = 0; j < childrenCopy.size(); ++j) {
                        if (j == i)
                            newItems.push_back(alt);
                        else
                            newItems.push_back(childrenCopy[j]);
                    }

                    auto concatNode = Regex::makeConcat(newItems);
                    auto normConcat = normalizeRegexAST(concatNode);
                    if (!normConcat) continue;

                    alts.push_back(normConcat);
                }

                if (alts.empty()) continue;

                auto unioned = Regex::makeUnion(alts);
                auto norm = normalizeRegexAST(unioned);
                if (!norm) continue;

                if (norm->cost() < r->cost())
                    return norm;
            }
        }
    }

    return r;
}

/*
 * prettifyRegexAST(r)
 *
 * Final cleanup phase:
 *   - normalize the expression
 *   - optionally apply distributive rewrite
 *   - re-normalize the rewritten form
 *   - choose the simpler of the two expressions
 *
 * Called after regex → automaton → regex conversion.
 */
std::shared_ptr<Regex> prettifyRegexAST(std::shared_ptr<Regex> r) {
    if (!r) return r;

    auto n = normalizeRegexAST(r);
    if (!n) return r;

    auto d = boundedDistribute(n);
    if (!d) return n;

    d = normalizeRegexAST(d);
    if (!d) return n;

    /* If equivalent structurally, keep normalized form */
    if (d->key() == n->key()) return n;

    /* Choose smaller expression */
    if (d->cost() < n->cost()) return d;
    return n;
}
