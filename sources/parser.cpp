#include <cstdlib>

#include "parser.h"
#include "exceptions.h"

Parser::Parser(std::istream &istream)
    :lexer_(istream)
{
    
}

int Parser::evalNextExpression()
{
    Token t = lexer_.nextToken();
    if (t.getTokenType() == TokenType::NUMBER) {
        return atoi(t.getContent().c_str());
    }
    throw InvalidInputException("Found an unexpected token: " + t.getContent());
}
