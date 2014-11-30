#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

class Parser
{
public:
    Parser(std::istream &istream);

    int evalNextExpression();

private:
    Lexer lexer_;
};

#endif