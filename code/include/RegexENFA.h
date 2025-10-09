#ifndef REGEX_ENFA_H
#define REGEX_ENFA_H

#include "Automaton.h"

#include <string>

Automaton regexToENFA(const std::string& inputBaseName);
Automaton eNFAtoNFA(const std::string& inputBaseName);

#endif