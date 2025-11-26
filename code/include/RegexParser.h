#ifndef REGEX_PARSER_H
#define REGEX_PARSER_H

#include "RegexAST.h"
#include <string>

/*
 * parseRegexToAST(s)
 *
 * Parses the input regular expression string into an AST (Regex).
 * The parser:
 *   - Inserts explicit concatenation operators.
 *   - Uses a shunting-yard style algorithm to handle precedence:
 *         '*'  >  concatenation  >  '|'
 *   - Builds Regex nodes for literals, union, concatenation, and star.
 *
 * The resulting AST is the structural input for normalization,
 * minimization, and regex-to-automaton conversions.
 */
std::shared_ptr<Regex> parseRegexToAST(const std::string &s);

#endif
