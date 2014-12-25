#include <cstdlib>
#include <iostream>

#include "parser.h"
#include "exceptions.h"

Parser::Parser(std::istream& istream, std::ostream &ostream)
    :lexer_(istream), ostream_(ostream)
{
    advance();
}

void Parser::advance()
{
    if (lexer_.hasNextToken()) {
        nextToken_ = lexer_.nextToken();
    } else {
        nextToken_ = Token(TokenType::END_OF_INPUT, "");
    }
}


void Parser::parseProgram()
{
    while (hasNextToken()) {
        skipNewLines();
        double value = evalNextExpression();
        ostream_ << value << std::endl;
        parseNewLine();
    }
}

double Parser::evalNextExpression()
{
    // First handle multiplication and parenthesis
    double value = evalNextTerm();

    // Next handle additions and subtractions. They have lower precedences, so they are handled AFTER.
    while (hasNextToken() && nextToken_.getTokenType() == TokenType::OPERATOR) {
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
    while (hasNextToken() && nextToken_.getTokenType() == TokenType::OPERATOR) {
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
        double value = atof(nextToken_.getContent().c_str());
        advance();
        return value;
    } else if (nextToken_.getTokenType()  == TokenType::OPERATOR
        && nextToken_.getContent() == "(") {
        return evalNextParenthesisFactor();
    } else if (nextToken_.getTokenType() == TokenType::IDENTIFIER) {
        return evalNextFunctionCall();
    } else {
        throw InvalidInputException("Found an unexpected token: " + nextToken_.getContent());
    }
}

double Parser::evalNextParenthesisFactor()
{
    // We match the '(' via advance; parse an expression; then match the ')'
    advance();

    double value = evalNextExpression();

    if (!hasNextToken()
            || nextToken_.getTokenType() != TokenType::OPERATOR
            || nextToken_.getContent() != ")") {
        throw InvalidInputException("Expected a closed parenthesis but found token: " + nextToken_.getContent());
    }
    advance();

    return value;
}

double Parser::evalNextFunctionCall() {
    // Match the function name
    std::string functionName = nextToken_.getContent();
    advance();

    // Is it a valid function?
    doubleToDoubleFunction f = lookupFunctionByName(functionName);

    // Eval its argument
    double argumentValue = evalNextFactor();

    // Call the function!
    return f(argumentValue);
}

void Parser::skipNewLines()
{
    while (hasNextToken() && nextToken_.getTokenType() == TokenType::END_OF_LINE) {
        advance();
    }
}

void Parser::parseNewLine()
{
    if (!hasNextToken()) {
        // Ok
        return;
    }
    if (nextToken_.getTokenType() != TokenType::END_OF_LINE) {
        throw InvalidInputException("Expected a newline");
    }
    advance();
}

Parser::doubleToDoubleFunction Parser::lookupFunctionByName(const std::string &name)
{
    auto it = functions_.find(name);
    if (it == functions_.end()) {
        throw UnknownFunctionName(name);
    }
    return it->second;
}
