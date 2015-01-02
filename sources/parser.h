#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <map>
#include <cmath>
#include <iostream>

#include "lexer.h"
#include "evaluation.h"

class Parser
{
public:
    Parser(std::istream& istream, std::ostream &ostream = std::cout);

    void parseProgram();
    double evalNextExpression();

private:
    static const int NUM_LOOK_AEAHD_TOKENS = 2;

    std::ostream &ostream_;
    Lexer lexer_;
    Token nextTokens_[NUM_LOOK_AEAHD_TOKENS];
    functionMap functions_ {
        {"exp", std::exp},
        {"log", std::log},
        {"sin", std::sin},
        {"cos", std::cos},
        {"tan", std::tan}
    };
    variablesMap variables_ {
        {"e", M_E},
        {"pi", M_PI}
    };

    inline const Token &getNextToken() const { return nextTokens_[0]; }
    inline const Token &getNextToken(int position) const { return nextTokens_[position]; }
    inline bool hasNextToken() const { return getNextToken().getTokenType() != TokenType::END_OF_INPUT; }
    inline bool hasNextTokens(int numTokens) const { return nextTokens_[numTokens].getTokenType() != TokenType::END_OF_INPUT; }

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
