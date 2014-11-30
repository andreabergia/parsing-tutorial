#ifndef LEXER_H
#define LEXER_H

#include "token.h"

class Lexer
{
public:
    explicit Lexer(std::istream& istream);
    Token nextToken();
    bool hasNextToken() const;

private:
    std::istream& istream_;
    char next_;
    bool atEof_;

    void advance();
    void skipSpaces();
    Token parseNumber();
    Token parseOperator();
};

#endif