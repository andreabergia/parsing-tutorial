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
    static const int NUM_LOOK_AEAHD_TOKENS = 2;

    // A doubleToDoubleFunction is a pointer to a function taking a double and returning a double
    using doubleToDoubleFunction = double(*)(double);

    std::ostream &ostream_;
    Lexer lexer_;
    Token nextTokens_[NUM_LOOK_AEAHD_TOKENS];
    std::map<std::string, doubleToDoubleFunction> functions_ {
        {"exp", std::exp},
        {"log", std::log},
        {"sin", std::sin},
        {"cos", std::cos},
        {"tan", std::tan}
    };
    std::map<std::string, double> variables_ {
        {"e", M_E},
        {"pi", M_PI}
    };

    inline const Token &getNextToken() const { return nextTokens_[0]; }
    inline bool hasNextToken() const { return getNextToken().getTokenType() != TokenType::END_OF_INPUT; }
    void advance();
    void match(TokenType tokenType, std::string content, std::string expected);

    void parseAssignment();
    void parseExpression();
    double evalNextTerm();
    double evalNextFactor();
    double evalNextParenthesisFactor();
    double evalNextFunctionCall();
    double evalNextVariable();
    void parseNewLine();
    void skipNewLines();
    doubleToDoubleFunction lookupFunctionByName(const std::string &name);
};

#endif
