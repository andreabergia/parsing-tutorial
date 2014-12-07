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

double Parser::evalNextExpression()
{
    // First handle multiplication and parenthesis
    double value = evalNextTerm();

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

double Parser::evalNextTerm()
{
    // First handle numbers and parenthesis
    double value = evalNextFactor();

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

double Parser::evalNextFactor()
{
    if (nextToken_.getTokenType() == TokenType::NUMBER) {
        double value = atoi(nextToken_.getContent().c_str());
        advance();
        return value;
    } else if (nextToken_.getTokenType()== TokenType::OPERATOR
        && nextToken_.getContent() == "(") {
        // We match the '(' via advance; parse an expression; then match the ')'
        advance();
        
        double value = evalNextExpression();

        if (!hasNextToken_ 
            || nextToken_.getTokenType() != TokenType::OPERATOR 
            || nextToken_.getContent() != ")") {
            throw InvalidInputException("Expected a closed parenthesis but found token: " + nextToken_.getContent());
        }
        advance();

        return value;
    }
    else {
        throw InvalidInputException("Found an unexpected token: " + nextToken_.getContent());
    }
}
