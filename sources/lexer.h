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
    Token parseNewLine();
    Token parseOperator();

    bool isIdentifierStart(char candidate) const;
    bool isIdentifierPart(char candidate) const;
    bool isEol(char candidate) const;

    Token parseIdentifier();
};

#endif
