#include "../include/Automaton.h"
#include "../include/RegexUtils.h"
#include "../include/RegexParser.h"
#include "../include/RegexNormalize.h"
#include "../include/RegexAST.h"

#include <fstream>
#include <string>
#include <memory>

/*
 * External functions used in the regex → automaton pipeline.
 * These operate on filenames (not raw strings), so temporary
 * files are used to integrate them into the minimization process.
 */
extern Automaton regexToENFA(const std::string& inputBaseName);
extern Automaton eNFAtoNFA(const std::string& inputBaseName);
extern std::string automatonToRegex(const Automaton& A);

/*
 * astToString(r)
 *
 * Convenience helper: return r->toString() or "" if null.
 * Not used extensively but kept for parity with earlier versions.
 */
static std::string astToString(std::shared_ptr<Regex> r) {
    if (!r) return "";
    return r->toString();
}

/*
 * minimizeRegex(regexInput)
 *
 * Performs full regular-expression minimization using:
 *
 *   1. Parse regex into AST
 *   2. AST-level simplification and normalization
 *   3. Write normalized regex to a temporary file
 *   4. Convert regex → ENFA
 *   5. Convert ENFA → NFA
 *   6. Determinise (NFA → DFA)
 *   7. Minimize the DFA
 *   8. Convert minimal DFA → regex (state elimination)
 *   9. AST-normalize final regex for canonical form
 *
 * Returns the final simplified string form. Empty language is
 * returned as "", and epsilon as "#", matching project conventions.
 */
std::string minimizeRegex(const std::string &regexInput) {

    /* Step 1: Parse and normalise AST of input regex */
    auto ast = parseRegexToAST(regexInput);
    ast = prettifyRegexAST(ast);

    /*
     * Step 2: Write the normalized AST to a temporary input file,
     * because regexToENFA() expects filename-based input.
     */
    std::string tmpBase = "tmp_min_regex_input";
    std::string inputPath = std::string("../../inputs/") + tmpBase + ".txt";

    {
        std::ofstream fout(inputPath);
        fout << ast->toString();
        fout.close();
    }

    /*
     * Step 3: Regex → ENFA → NFA
     */
    Automaton enfa = regexToENFA(tmpBase);
    Automaton nfa  = eNFAtoNFA(tmpBase);

    /*
     * Step 4: Determinise and minimize the automaton
     */
    Automaton dfa    = Automaton::determinise(nfa);
    Automaton minDFA = Automaton::minimize(dfa);

    /*
     * Step 5: Convert minimal DFA back to a regex
     */
    std::string rawRegex = automatonToRegex(minDFA);

    /*
     * Step 6: Final round of AST parsing and prettification
     */
    auto outAst = parseRegexToAST(rawRegex);
    outAst = prettifyRegexAST(outAst);

    std::string finalStr = outAst->toString();

    /*
     * Step 7: Special-case returns
     */
    if (finalStr.empty()) return "";   // empty language
    if (finalStr == "#")   return "#"; // epsilon

    return finalStr;
}
