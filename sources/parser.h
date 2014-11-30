#ifndef PARSER_H
#define PARSER_H

#include <memory>

#include "lexer.h"

class Parser
{
public:
    Parser(std::istream& istream);

    int evalNextExpression();

private:
    Lexer lexer_;
    Token nextToken_{TokenType::END_OF_INPUT, ""};
    bool hasNextToken_{false};

    void advance();

    int evalNextTerm();
    int evalNextFactor();
};

#endif