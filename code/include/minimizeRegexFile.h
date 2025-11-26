#ifndef MINIMIZE_REGEX_FILE_H
#define MINIMIZE_REGEX_FILE_H

/*
 * minimizeRegexFromFile()
 *
 * Reads a regular expression from a text file in the inputs directory,
 * minimizes it using the full AST + automata pipeline, writes the minimized
 * regex into the outputs directory, and generates an image of the resulting
 * minimal DFA. 
 *
 * This function is invoked from main.cpp under menu option 10.
 */
void minimizeRegexFromFile();

#endif
