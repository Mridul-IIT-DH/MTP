#ifndef REGEX_AST_H
#define REGEX_AST_H

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

/*
 * RKind
 *
 * Enumeration of node types that appear in the abstract syntax tree (AST)
 * representation of a regular expression:
 *   - EMPTYSET : represents the empty language (φ)
 *   - EPS      : represents epsilon (#)
 *   - LITERAL  : a single character symbol
 *   - UNION    : alternation (r | s)
 *   - CONCAT   : concatenation (rs)
 *   - STAR     : Kleene star (r*)
 */
enum class RKind { EMPTYSET, EPS, LITERAL, UNION, CONCAT, STAR };

/*
 * Regex
 *
 * Node structure for the regular expression AST. Each node represents
 * a single syntactic operation. Depending on the kind of node:
 *
 *   - LITERAL  : 'literal' stores the character
 *   - UNION    : 'children' holds each alternative
 *   - CONCAT   : 'children' holds each concatenation factor
 *   - STAR     : 'child'    holds the repeated subexpression
 *   - EPS and EMPTYSET have no child data
 *
 * Factory functions (makeEmpty, makeEps, makeLit, etc.) are used to build
 * these nodes consistently. The toString(), key(), and cost() methods
 * support printing, canonicalization, and heuristic comparisons.
 */
struct Regex {
    RKind kind;                                // Type of AST node
    char literal;                              // Used only when kind == LITERAL
    std::vector<std::shared_ptr<Regex>> children; // Used for UNION and CONCAT
    std::shared_ptr<Regex> child;              // Used for STAR

    Regex(RKind k = RKind::EMPTYSET);

    static std::shared_ptr<Regex> makeEmpty();
    static std::shared_ptr<Regex> makeEps();
    static std::shared_ptr<Regex> makeLit(char c);
    static std::shared_ptr<Regex> makeUnion(std::vector<std::shared_ptr<Regex>> v);
    static std::shared_ptr<Regex> makeConcat(std::vector<std::shared_ptr<Regex>> v);
    static std::shared_ptr<Regex> makeStar(std::shared_ptr<Regex> r);

    /*
     * toString()
     * Converts the AST subtree rooted at this node into a printable
     * regex string following the project's formatting rules.
     */
    std::string toString() const;

    /*
     * key()
     * Produces a canonical string uniquely describing the structure
     * of the AST node. Used for sorting child vectors and detecting
     * structural equivalence between ASTs.
     */
    std::string key() const;

    /*
     * cost()
     * Computes a heuristic size/cost measure used by the normalizer
     * when comparing alternative rewritings (e.g., distributed vs.
     * factored forms). Lower cost indicates a simpler expression.
     */
    int cost() const;
};

#endif
