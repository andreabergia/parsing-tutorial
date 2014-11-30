#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

class Parser
{
public:
    Parser(std::istream& istream);

    int evalNextExpression();

private:
    Lexer lexer_;

    int evalNextTerm();
    int evalNextFactor();
    int handleAdditionSubtraction(int currValue);
    int handleMultiplicationDivision(int currValue);
};

#endif