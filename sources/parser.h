#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <map>
#include <cmath>
#include <iostream>

#include "lexer.h"
#include "evaluation.h"
#include "node.h"

class Parser
{
public:
    Parser(std::istream& istream, std::ostream &ostream = std::cout);

    void parseProgram();

    // Public only to simplify unit tests; in real code they would be private
    NodePtr getNextExpressionNode();
    double evalNode(NodePtr node);

private:
    static const int NUM_LOOK_AEAHD_TOKENS = 2;

    std::ostream &ostream_;
    Lexer lexer_;
    Token nextTokens_[NUM_LOOK_AEAHD_TOKENS];
    userFunctionsMap userDefinedFunctions_;
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
    NodePtr evalNextTerm();
    NodePtr evalNextFactor();
    NodePtr evalNextParenthesisFactor();
    NodePtr evalNextFunctionCall();
    NodePtr evalNextVariable();
    void parseNewLine();
    void skipNewLines();
};

#endif
