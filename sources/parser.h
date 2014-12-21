#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <map>
#include <cmath>
#include <iostream>

#include "lexer.h"

class Parser
{
public:
    Parser(std::istream& istream, std::ostream &ostream = std::cout);

    void parseProgram();
    double evalNextExpression();

private:
    // A doubleToDoubleFunction is a pointer to a function taking a double and returning a double
    using doubleToDoubleFunction = double(*)(double);

    std::ostream &ostream_;
    Lexer lexer_;
    Token nextToken_{TokenType::END_OF_INPUT, ""};
    bool hasNextToken_{false};
    std::map<std::string, doubleToDoubleFunction> functions_ {
        {"exp", std::exp},
        {"log", std::log},
        {"sin", std::sin},
        {"cos", std::cos},
        {"tan", std::tan}
    };

    void advance();

    double evalNextTerm();
    double evalNextFactor();
    double evalNextParenthesisFactor();
    double evalNextFunctionCall();
    void parseNewLine();
    void skipNewLines();
    doubleToDoubleFunction lookupFunctionByName(const std::string &name);
};

#endif
