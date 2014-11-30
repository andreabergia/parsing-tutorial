#include <istream>
#include <stdexcept>
#include <cctype>
#include <string>
#include <set>

#include "lexer.h"

static std::set<std::string> validOperators = {"+", "-", "*", "/", "(", ")"};

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
    return Token(TokenType::NUMBER, num);
}

Token Lexer::parseOperator()
{
    std::string nextAsString = std::string{next_};
    if (!validOperators.count(nextAsString)) {
        throw InvalidInputException("Invalid operator type: " + next_);
    }
    Token result = Token(TokenType::OPERATOR, nextAsString);
    advance();
    skipSpaces();
    return result;
}