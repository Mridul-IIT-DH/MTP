#ifndef REGEX_NORMALIZE_H
#define REGEX_NORMALIZE_H

#include "RegexAST.h"
#include <memory>

/*
 * normalizeRegexAST(r)
 *
 * Applies structural simplifications to the AST:
 *   - Flattens CONCAT and UNION nodes
 *   - Eliminates redundant EPS nodes
 *   - Removes EMPTYSET where appropriate
 *   - Simplifies (r*)* → r*
 *   - Orders UNION children in a canonical way
 *
 * This function is called by the regex simplification and
 * minimization pipeline before and after distribution.
 */
std::shared_ptr<Regex> normalizeRegexAST(std::shared_ptr<Regex> r);

/*
 * boundedDistribute(r)
 *
 * Tries a small controlled version of distributive expansion:
 *
 *      r = (x | y) z   →   xz | yz
 *
 * but only when:
 *   - the UNION has at most 2 children, and
 *   - the resulting expression reduces overall cost.
 *
 * This prevents exponential blow-ups while allowing useful
 * factorization and simplification to occur.
 */
std::shared_ptr<Regex> boundedDistribute(std::shared_ptr<Regex> r);

/*
 * prettifyRegexAST(r)
 *
 * High-level normalization step combining:
 *   - normalizeRegexAST()
 *   - boundedDistribute()
 *   - cost-based comparison between normalized and distributed forms
 *
 * Returns whichever form is structurally simpler.
 * Used as the final cleanup step after regex → DFA → regex conversion.
 */
std::shared_ptr<Regex> prettifyRegexAST(std::shared_ptr<Regex> r);

#endif
