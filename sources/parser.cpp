#include <cstdlib>
#include <iostream>

#include "parser.h"

Parser::Parser(std::istream& istream, std::ostream &ostream)
    :lexer_(istream), ostream_(ostream)
{
    // Fetch look-ahead tokens
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

        // Assignment?
        if (hasNextTokens(2)
                && getNextToken().getTokenType() == TokenType::IDENTIFIER
                && getNextToken(1).getTokenType() == TokenType::OPERATOR
                && getNextToken(1).getContent() == "=") {
            parseAssignment();
        } else {
            parseExpression();
        }
        parseNewLine();
    }
}

void Parser::parseAssignment()
{
    // Match variable name
    if (!hasNextToken() || getNextToken().getTokenType() != TokenType::IDENTIFIER) {
        throw InvalidInputException("Found an unexpected token: " + getNextToken().getContent());
    }
    std::string variableName = getNextToken().getContent();
    advance();

    // Match =
    match(TokenType::OPERATOR, "=", "the assigment operator =");

    // Get the expression as a node, evaluate it and save the variable value
    NodePtr node = getNextExpressionNode();
    variables_[variableName] = evalNode(node);
};

void Parser::parseExpression()
{
    NodePtr node = getNextExpressionNode();
    ostream_ << evalNode(node) << std::endl;
}

NodePtr Parser::getNextExpressionNode()
{
    // First handle multiplication and parenthesis
    NodePtr node = evalNextTerm();

    // Next handle additions and subtractions. They have lower precedences, so they are handled AFTER.
    while (hasNextToken() && getNextToken().getTokenType() == TokenType::OPERATOR) {
        if (getNextToken().getContent() == "+") {
            advance();
            node = NodePtr(new AdditionNode(node, evalNextTerm()));
        } else if (getNextToken().getContent() == "-") {
            advance();
            node = NodePtr(new SubtractionNode(node, evalNextTerm()));
        } else {
            // An operator, but not '+' or '-': let the caller handle it.
            break;
        }
    }

    return node;
}

NodePtr Parser::evalNextTerm()
{
    // First handle numbers and parenthesis
    NodePtr node = evalNextFactor();

    // Next handle multiplications and divisions
    while (hasNextToken() && getNextToken().getTokenType() == TokenType::OPERATOR) {
        if (getNextToken().getContent() == "*") {
            advance();
            node = NodePtr(new MultiplicationNode(node, evalNextFactor()));
        } else if (getNextToken().getContent() == "/") {
            advance();
            node = NodePtr(new DivisionNode(node, evalNextFactor()));
        } else {
            // An operator, but not '*' or '/': let the caller handle it.
            break;
        }
    }

    return node;
}

NodePtr Parser::evalNextFactor()
{
    if (getNextToken().getTokenType() == TokenType::NUMBER) {
        double value = atof(getNextToken().getContent().c_str());
        advance();
        return NodePtr(new NumberNode(value));
    } else if (getNextToken().getTokenType()  == TokenType::OPERATOR
        && getNextToken().getContent() == "(") {
        return evalNextParenthesisFactor();
    } else if (hasNextTokens(2)
            && getNextToken().getTokenType() == TokenType::IDENTIFIER
            && getNextToken(1).getTokenType() == TokenType::OPERATOR
            && getNextToken(1).getContent() == "(") {
        return evalNextFunctionCall();
    } else if (getNextToken().getTokenType() == TokenType::IDENTIFIER) {
        return evalNextVariable();
    } else {
        throw InvalidInputException("Found an unexpected token: " + getNextToken().getContent());
    }
}

NodePtr Parser::evalNextParenthesisFactor()
{
    // Match the '('; parse an expression; then match the ')'
    match(TokenType::OPERATOR, "(", "an open parenthesis");
    NodePtr node = getNextExpressionNode();
    match(TokenType::OPERATOR, ")", "a closed parenthesis");

    return node;
}

NodePtr Parser::evalNextFunctionCall() {
    // Match the function name and the open parenthesis
    std::string functionName = getNextToken().getContent();
    advance();
    match(TokenType::OPERATOR, "(", "an open parenthesis");

    // Parse its argument as a node and match the closed parenthesis
    NodePtr argument = getNextExpressionNode();
    match(TokenType::OPERATOR, ")", "a closed parenthesis");

    // Make a function call node
    return NodePtr(new FunctionCallNode(functionName, argument));
}

NodePtr Parser::evalNextVariable() {
    // Match the variable name
    std::string variableName = getNextToken().getContent();
    advance();

    // Make a variable access node
    return NodePtr(new VariableNode(variableName));
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

double Parser::evalNode(NodePtr node)
{
    EvaluationContext evaluationContext(userDefinedFunctions_, variables_);
    return node->eval(evaluationContext);
}
