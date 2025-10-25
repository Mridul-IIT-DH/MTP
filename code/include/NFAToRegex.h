#ifndef NFA_TO_REGEX_H
#define NFA_TO_REGEX_H

#include "Automaton.h"

#include <string>

std::string automatonToRegex(const Automaton& A);

#endif