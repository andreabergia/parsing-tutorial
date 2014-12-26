#include <istream>
#include <cctype>
#include <string>
#include <set>

#include "lexer.h"
#include "exceptions.h"

static std::set<std::string> validOperators = {"+", "-", "*", "/", "(", ")", "="};

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
    while (!atEof_ && !isEol(next_) && std::isspace(next_)) {
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
        return Token(TokenType::END_OF_INPUT, "");
    }

    if (std::isdigit(next_)) {
        return parseNumber();
    } else if (isIdentifierStart(next_)) {
        return parseIdentifier();
    } else if (isEol(next_)) {
        return parseNewLine();
    } else {
        return parseOperator();
    }
}

Token Lexer::parseNumber()
{
    std::string num;

    // Integer part
    while (!atEof_ && isdigit(next_)) {
        num += next_;
        advance();
    }

    // Dot and floating part?
    if (!atEof_ && next_ == '.') {
        num += '.';
        advance();
        while (!atEof_ && isdigit(next_)) {
            num += next_;
            advance();
        }
    }

    skipSpaces();
    return Token(TokenType::NUMBER, num);
}

Token Lexer::parseIdentifier()
{
    // Add the first character to the identifier
    std::string identifier{next_};
    advance();

    // Match more identifier parts
    while (!atEof_ && isIdentifierPart(next_)) {
        identifier += next_;
        advance();
    }

    skipSpaces();
    return Token(TokenType::IDENTIFIER, identifier);
}

Token Lexer::parseNewLine()
{
    if (next_ == '\r') {
        advance();
        if (next_ != '\n') {
            throw InvalidInputException("Expected a \n after a \r");
        }
    }
    advance();
    skipSpaces();
    return Token(TokenType::END_OF_LINE, "");
}

Token Lexer::parseOperator()
{
    std::string operatorText = std::string{next_};
    if (!validOperators.count(operatorText)) {
        throw InvalidInputException("Invalid operator type: " + operatorText);
    }
    advance();
    skipSpaces();
    return Token(TokenType::OPERATOR, operatorText);
}

bool Lexer::isIdentifierStart(char candidate) const
{
    return std::isalpha(candidate);
}

bool Lexer::isIdentifierPart(char candidate) const
{
    return isIdentifierStart(candidate) || std::isdigit(candidate);
}

bool Lexer::isEol(char candidate) const
{
    return candidate == '\n' || candidate == '\r';
}
