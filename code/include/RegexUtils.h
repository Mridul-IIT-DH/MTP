#ifndef REGEX_UTILS_H
#define REGEX_UTILS_H

#include <string>

/*
 * minimizeRegex(regexInput)
 *
 * Takes a regular expression as a string and performs full
 * minimization using the project's regex pipeline:
 *
 *   1. Parse into AST.
 *   2. Normalize and simplify (AST-level reduction).
 *   3. Convert to ENFA, then NFA.
 *   4. Determinise to DFA.
 *   5. Minimize the DFA.
 *   6. Convert the minimal DFA back to a regex.
 *   7. Normalize again for final canonical form.
 *
 * Returns the minimized regular expression as a string.
 * Used internally by multiple features, including option 10.
 */
std::string minimizeRegex(const std::string &regexInput);

#endif
