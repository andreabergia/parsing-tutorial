#include <cstdlib>

#include "parser.h"
#include "exceptions.h"

Parser::Parser(std::istream& istream)
    :lexer_(istream)
{
}

int Parser::evalNextExpression()
{
    // First handle multiplication and parenthesis
    int value = evalNextTerm();

    // Next handle additions and subtractions. They have lower precedences, so they are handled AFTER.
    while (lexer_.hasNextToken()) {
       value = handleAdditionSubtraction(value);
    }
    return value;
}

int Parser::evalNextTerm()
{
    // First handle numbers and parenthesis
    int value = evalNextFactor();

    // Next handle multiplications and divisions.
    while (lexer_.hasNextToken()) {
        value = handleMultiplicationDivision(value);
    }
    return value;
}

int Parser::evalNextFactor()
{
    Token t = lexer_.nextToken();
    if (t.getTokenType() == TokenType::NUMBER) {
        return atoi(t.getContent().c_str());
    }
    throw InvalidInputException("Found an unexpected token: " + t.getContent());
}

int Parser::handleAdditionSubtraction(int currValue)
{
    Token t = lexer_.nextToken();
    if (t.getTokenType() == TokenType::OPERATOR) {
        if (t.getContent() == "+") {
            return currValue + evalNextTerm();
        } else if (t.getContent() == "-") {
            return currValue - evalNextTerm();
        } else {
            throw InvalidInputException("Found an invalid operator: " + t.getContent());
        }
    } else {
        throw InvalidInputException("Found an unexpected token: " + t.getContent());
    }
}

int Parser::handleMultiplicationDivision(int currValue)
{
    Token t = lexer_.nextToken();
    if (t.getTokenType() == TokenType::OPERATOR) {
        if (t.getContent() == "*") {
            return currValue + evalNextTerm();
        }
        else if (t.getContent() == "/") {
            return currValue - evalNextTerm();
        }
        else {
            throw InvalidInputException("Found an invalid operator: " + t.getContent());
        }
    }
    else {
        throw InvalidInputException("Found an unexpected token: " + t.getContent());
    }
}