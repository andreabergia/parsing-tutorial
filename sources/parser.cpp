#include <cstdlib>
#include <iostream>

#include "parser.h"
#include "exceptions.h"

Parser::Parser(std::istream& istream, std::ostream &ostream)
    :lexer_(istream), ostream_(ostream)
{
    for (int i = 0; i < NUM_LOOK_AEAHD_TOKENS; ++i) {
        if (lexer_.hasNextToken()) {
            nextTokens_[i] = lexer_.nextToken();
        }
    }
}

void Parser::advance()
{
    // Shift tokens one position back
    for (int i = 0; i < NUM_LOOK_AEAHD_TOKENS - 1; ++i) {
        nextTokens_[i] = nextTokens_[i + 1];
    }

    // Set last available token
    nextTokens_[NUM_LOOK_AEAHD_TOKENS - 1] =
            lexer_.hasNextToken()
                ? lexer_.nextToken()
                : Token{TokenType::END_OF_INPUT, ""};
}

void Parser::match(TokenType tokenType, std::string content, std::string expected) {
    if (!hasNextToken()
            || getNextToken().getTokenType() != tokenType
            || getNextToken().getContent() != content) {
        throw InvalidInputException("Expected " + expected + " but found token: " + getNextToken().getContent());
    }
    advance();
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
    while (hasNextToken() && getNextToken().getTokenType() == TokenType::OPERATOR) {
        if (getNextToken().getContent() == "+") {
            advance();
            value += evalNextTerm();
        } else if (getNextToken().getContent() == "-") {
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
    while (hasNextToken() && getNextToken().getTokenType() == TokenType::OPERATOR) {
        if (getNextToken().getContent() == "*") {
            advance();
            value *= evalNextFactor();
        } else if (getNextToken().getContent() == "/") {
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
    if (getNextToken().getTokenType() == TokenType::NUMBER) {
        double value = atof(getNextToken().getContent().c_str());
        advance();
        return value;
    } else if (getNextToken().getTokenType()  == TokenType::OPERATOR
        && getNextToken().getContent() == "(") {
        return evalNextParenthesisFactor();
    } else if (getNextToken().getTokenType() == TokenType::IDENTIFIER
            && nextTokens_[1].getTokenType() == TokenType::OPERATOR
            && nextTokens_[1].getContent() == "(") {
        return evalNextFunctionCall();
    } else if (getNextToken().getTokenType() == TokenType::IDENTIFIER) {
        return evalNextVariable();
    } else {
        throw InvalidInputException("Found an unexpected token: " + getNextToken().getContent());
    }
}

double Parser::evalNextParenthesisFactor()
{
    // Match the '('; parse an expression; then match the ')'
    match(TokenType::OPERATOR, "(", "an open parenthesis");
    double value = evalNextExpression();
    match(TokenType::OPERATOR, ")", "a closed parenthesis");

    return value;
}

double Parser::evalNextFunctionCall() {
    // Match the function name and the open parenthesis
    std::string functionName = getNextToken().getContent();
    advance();
    match(TokenType::OPERATOR, "(", "an open parenthesis");

    // Is it a valid function?
    doubleToDoubleFunction f = lookupFunctionByName(functionName);

    // Eval its argument and match the closed parenthesis
    double argumentValue = evalNextExpression();
    match(TokenType::OPERATOR, ")", "a closed parenthesis");

    // Call the function!
    return f(argumentValue);
}

double Parser::evalNextVariable() {
    // Match the variable name
    std::string variableName = getNextToken().getContent();
    advance();

    // Lookup its value
    auto it = variables_.find(variableName);
    if (it == variables_.end()) {
        throw UnknownVariableName(variableName);
    }
    return it->second;
}

void Parser::skipNewLines()
{
    while (hasNextToken() && getNextToken().getTokenType() == TokenType::END_OF_LINE) {
        advance();
    }
}

void Parser::parseNewLine()
{
    if (!hasNextToken()) {
        // Ok
        return;
    }
    match(TokenType::END_OF_LINE, "", "newline");
}

Parser::doubleToDoubleFunction Parser::lookupFunctionByName(const std::string &name)
{
    auto it = functions_.find(name);
    if (it == functions_.end()) {
        throw UnknownFunctionName(name);
    }
    return it->second;
}
