#include <istream>
#include <stdexcept>
#include <cctype>

#include "lexer.h"

Lexer::Lexer(std::istream& istream)
    : istream_(istream), atEof_(false)
{
    advance();
    skipSpaces();
}

void Lexer::advance()
{
    next_ = static_cast<char>(istream_.get());
    atEof_ = istream_.eof();
}

void Lexer::skipSpaces()
{
    while (!atEof_ && std::isspace(next_)) {
        advance();
    }
}

bool Lexer::hasNextToken() const
{
    return !atEof_;
}

Token Lexer::nextToken()
{
    if (atEof_) {
        throw std::runtime_error("EOF");
    }

    if (std::isdigit(next_)) {
        return parseNumber();
    } else {
        return parseOperator();
    }
}

Token Lexer::parseNumber()
{
    std::string num;
    while (!atEof_ && isdigit(next_)) {
        num += next_;
        advance();
    }

    skipSpaces();
    return Token{NUMBER, num};
}

Token Lexer::parseOperator()
{
    Token result = Token{OPERATOR, std::string{next_}};
    advance();
    skipSpaces();
    return result;
}