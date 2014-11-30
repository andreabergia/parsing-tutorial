#include <cstdlib>

#include "parser.h"
#include "exceptions.h"

Parser::Parser(std::istream& istream)
    :lexer_(istream)
{
    advance();
}

void Parser::advance()
{
    if (lexer_.hasNextToken()) {
        nextToken_ = lexer_.nextToken();
        hasNextToken_ = true;
    } else {
        nextToken_ = Token(TokenType::END_OF_INPUT, "");
        hasNextToken_ = false;
    }
}

int Parser::evalNextExpression()
{
    // First handle multiplication and parenthesis
    int value = evalNextTerm();

    // Next handle additions and subtractions. They have lower precedences, so they are handled AFTER.
    while (hasNextToken_ && nextToken_.getTokenType() == TokenType::OPERATOR) {
        if (nextToken_.getContent() == "+") {
            advance();
            value += evalNextTerm();
        } else if (nextToken_.getContent() == "-") {
            advance();
            value -= evalNextTerm();
        } else {
            // An operator, but not '+' or '-': let the caller handle it.
            break;
        }
    }

    return value;
}

int Parser::evalNextTerm()
{
    // First handle numbers and parenthesis
    int value = evalNextFactor();

    // Next handle multiplications and divisions
    while (hasNextToken_ && nextToken_.getTokenType() == TokenType::OPERATOR) {
        if (nextToken_.getContent() == "*") {
            advance();
            value *= evalNextFactor();
        } else if (nextToken_.getContent() == "/") {
            advance();
            value /= evalNextFactor();
        } else {
            // An operator, but not '*' or '/': let the caller handle it.
            break;
        }
    }

    return value;
}

int Parser::evalNextFactor()
{
    Token currToken = nextToken_;
    if (currToken.getTokenType() == TokenType::NUMBER) {
        advance();
        return atoi(currToken.getContent().c_str());
    }
    throw InvalidInputException("Found an unexpected token: " + currToken.getContent());
}