#include "../include/RegexAST.h"
#include <sstream>

/*
 * Constructor: initialize node with a specific kind.
 * Literal is set to 0 by default; it is only used for LITERAL nodes.
 */
Regex::Regex(RKind k) : kind(k), literal(0) {}

/*
 * Factory: create EMPTYSET node (represents φ).
 */
std::shared_ptr<Regex> Regex::makeEmpty() {
    return std::make_shared<Regex>(RKind::EMPTYSET);
}

/*
 * Factory: create EPS node (represents epsilon, '#').
 */
std::shared_ptr<Regex> Regex::makeEps() {
    return std::make_shared<Regex>(RKind::EPS);
}

/*
 * Factory: create literal node storing a single character.
 */
std::shared_ptr<Regex> Regex::makeLit(char c) {
    auto r = std::make_shared<Regex>(RKind::LITERAL);
    r->literal = c;
    return r;
}

/*
 * Factory: create UNION node with a vector of child expressions.
 */
std::shared_ptr<Regex> Regex::makeUnion(std::vector<std::shared_ptr<Regex>> v) {
    auto r = std::make_shared<Regex>(RKind::UNION);
    r->children = std::move(v);
    return r;
}

/*
 * Factory: create CONCAT node with a vector of child expressions.
 */
std::shared_ptr<Regex> Regex::makeConcat(std::vector<std::shared_ptr<Regex>> v) {
    auto r = std::make_shared<Regex>(RKind::CONCAT);
    r->children = std::move(v);
    return r;
}

/*
 * Factory: create STAR node with a single child expression.
 */
std::shared_ptr<Regex> Regex::makeStar(std::shared_ptr<Regex> c) {
    auto r = std::make_shared<Regex>(RKind::STAR);
    r->child = c;
    return r;
}

/*
 * Helper: print child with parentheses when needed.
 * (Used internally for generating string forms.)
 */
static void parenIfNeeded(std::ostringstream &os, const std::shared_ptr<Regex>& r) {
    if (r->kind == RKind::UNION || r->kind == RKind::CONCAT) {
        os << "(" << r->toString() << ")";
    } else {
        os << r->toString();
    }
}

/*
 * toString()
 *
 * Converts an AST node into its string representation.
 * This matches the project’s regex formatting conventions.
 */
std::string Regex::toString() const {
    std::ostringstream os;

    switch (kind) {

        case RKind::EMPTYSET:
            return "";     // empty language is printed as empty string

        case RKind::EPS:
            return "#";    // epsilon is printed as "#"

        case RKind::LITERAL: {
            os << literal;
            return os.str();
        }

        case RKind::UNION: {
            for (size_t i = 0; i < children.size(); ++i) {
                if (i) os << "|";
                os << children[i]->toString();
            }
            return os.str();
        }

        case RKind::CONCAT: {
            for (auto &c : children) {
                if (c->kind == RKind::UNION)
                    os << "(" << c->toString() << ")";
                else
                    os << c->toString();
            }
            return os.str();
        }

        case RKind::STAR: {
            auto &c = child;
            if (c->kind == RKind::LITERAL || c->kind == RKind::EPS) {
                os << c->toString() << "*";
            } else {
                os << "(" << c->toString() << ")*";
            }
            return os.str();
        }
    }

    return "";
}

/*
 * key()
 *
 * Produces a canonical structural string for this node.
 * Used for:
 *   - sorting UNION/CONCAT children
 *   - testing structural equality between subtrees
 */
std::string Regex::key() const {
    std::ostringstream os;
    os << static_cast<int>(kind) << ":";

    switch (kind) {
        case RKind::LITERAL:
            os << literal;
            break;

        case RKind::EPS:
            os << "#";
            break;

        case RKind::EMPTYSET:
            os << "φ";
            break;

        case RKind::UNION:
        case RKind::CONCAT:
            for (auto &c : children)
                os << c->key() << ",";
            break;

        case RKind::STAR:
            os << child->key();
            break;
    }

    return os.str();
}

/*
 * cost()
 *
 * Returns a heuristic complexity measure for the subtree.
 * Used inside the normalizer to choose between alternative
 * rewritings (e.g., factored vs. distributed).
 *
 * Lower cost = preferred expression.
 */
int Regex::cost() const {
    switch (kind) {
        case RKind::EMPTYSET: return 1;
        case RKind::EPS:      return 1;
        case RKind::LITERAL:  return 1;

        case RKind::STAR:
            return 1 + child->cost();

        case RKind::CONCAT: {
            int s = 0;
            for (auto &c : children) s += c->cost();
            return s + (int)children.size() - 1;
        }

        case RKind::UNION: {
            int s = 0;
            for (auto &c : children) s += c->cost();
            return s + 3;   // UNION weighted slightly higher
        }
    }

    return 1;
}
