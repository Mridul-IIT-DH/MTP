#include "../include/RegexParser.h"
#include <stack>
#include <cctype>

/*
 * prec(op)
 *
 * Precedence table for regex operators:
 *   '*'  : highest
 *   '.'  : concatenation
 *   '|'  : union
 */
static int prec(char op) {
    if (op == '*') return 3;
    if (op == '.') return 2;
    if (op == '|') return 1;
    return 0;
}

/*
 * applyOp(vals, op)
 *
 * Applies an operator to the top elements of the value stack.
 * Uses Regex factory functions and flattens UNION and CONCAT nodes
 * to keep the AST compact.
 */
static void applyOp(std::stack<std::shared_ptr<Regex>> &vals, char op) {
    if (op == '*') {
        auto v = vals.top(); vals.pop();
        vals.push(Regex::makeStar(v));
    }
    else if (op == '.') {
        auto b = vals.top(); vals.pop();
        auto a = vals.top(); vals.pop();

        std::vector<std::shared_ptr<Regex>> items;

        /* flatten concatenation on left */
        if (a->kind == RKind::CONCAT)
            items.insert(items.end(), a->children.begin(), a->children.end());
        else
            items.push_back(a);

        /* flatten concatenation on right */
        if (b->kind == RKind::CONCAT)
            items.insert(items.end(), b->children.begin(), b->children.end());
        else
            items.push_back(b);

        vals.push(Regex::makeConcat(items));
    }
    else if (op == '|') {
        auto b = vals.top(); vals.pop();
        auto a = vals.top(); vals.pop();

        std::vector<std::shared_ptr<Regex>> items;

        /* flatten UNION on left */
        if (a->kind == RKind::UNION)
            items.insert(items.end(), a->children.begin(), a->children.end());
        else
            items.push_back(a);

        /* flatten UNION on right */
        if (b->kind == RKind::UNION)
            items.insert(items.end(), b->children.begin(), b->children.end());
        else
            items.push_back(b);

        vals.push(Regex::makeUnion(items));
    }
}

/*
 * insertConcat(in)
 *
 * Inserts explicit '.' operators to represent concatenation.
 * Concatenation is implied in standard regex syntax but required
 * here for parsing with precedence.
 *
 * Example:
 *   a(b|c)*d → a.(b|c)*.d
 */
std::string insertConcat(const std::string &in) {
    std::string out;

    for (size_t i = 0; i < in.size(); ++i) {
        char c1 = in[i];
        out.push_back(c1);

        if (i + 1 < in.size()) {
            char c2 = in[i+1];

            bool left =
                (isalnum(c1) || c1 == '#' || c1 == ')' || c1 == '*');

            bool right =
                (isalnum(c2) || c2 == '#' || c2 == '(');

            if (left && right)
                out.push_back('.');
        }
    }
    return out;
}

/*
 * parseRegexToAST(s_in)
 *
 * Full regex parser using a shunting-yard-style algorithm.
 *
 * Steps:
 *   1. Insert explicit concatenation operators.
 *   2. Read characters of the input:
 *        - literals and '#' pushed to value stack
 *        - '(' pushed to operator stack
 *        - ')' causes operator application until '('
 *        - '*', '|', '.' checked against precedence table
 *   3. After scanning, apply remaining operators
 *   4. Resulting stack top is the AST root
 */
std::shared_ptr<Regex> parseRegexToAST(const std::string &s_in) {
    std::string s = insertConcat(s_in);

    std::stack<char> ops;
    std::stack<std::shared_ptr<Regex>> vals;

    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];

        if (isalnum(c) || c == '#') {
            if (c == '#')
                vals.push(Regex::makeEps());
            else
                vals.push(Regex::makeLit(c));
        }

        else if (c == '(') {
            ops.push(c);
        }

        else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                char op = ops.top(); ops.pop();
                applyOp(vals, op);
            }
            if (!ops.empty() && ops.top() == '(')
                ops.pop();
        }

        else if (c == '*' || c == '|' || c == '.') {
            while (!ops.empty() && ops.top() != '(' &&
                   prec(ops.top()) >= prec(c)) {
                char op = ops.top(); ops.pop();
                applyOp(vals, op);
            }
            ops.push(c);
        }

        else {
            /* ignore unknown/non-regex characters */
        }
    }

    while (!ops.empty()) {
        char op = ops.top(); ops.pop();
        applyOp(vals, op);
    }

    if (vals.empty())
        return Regex::makeEmpty();

    return vals.top();
}
