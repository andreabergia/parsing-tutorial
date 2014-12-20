#include <sstream>

#include "lest.hpp"

#include "lexer.h"
#include "exceptions.h"

const lest::test testLexer[] = {
    CASE("lexing '1'") {
        std::istringstream input{"1"};
        Lexer lexer(input);
        EXPECT(lexer.hasNextToken());

        EXPECT(lexer.nextToken() == Token(TokenType::NUMBER, "1"));

        EXPECT_NOT(lexer.hasNextToken());
    },

    CASE("lexing '3.14'") {
        std::istringstream input{"3.14"};
        Lexer lexer(input);
        EXPECT(lexer.hasNextToken());

        EXPECT(lexer.nextToken() == Token(TokenType::NUMBER, "3.14"));

        EXPECT_NOT(lexer.hasNextToken());
    },

    CASE("lexing 'sin x'") {
        std::istringstream input{"sin x"};
        Lexer lexer(input);
        EXPECT(lexer.hasNextToken());

        EXPECT(lexer.nextToken() == Token(TokenType::IDENTIFIER, "sin"));
        EXPECT(lexer.hasNextToken());

        EXPECT(lexer.nextToken() == Token(TokenType::IDENTIFIER, "x"));
        EXPECT_NOT(lexer.hasNextToken());
    },

    CASE("lexing '1 + 23'") {
        std::istringstream input{"1 + 23"};
        Lexer lexer(input);
        EXPECT(lexer.hasNextToken());

        EXPECT(lexer.nextToken() == Token(TokenType::NUMBER, "1"));
        EXPECT(lexer.hasNextToken());

        EXPECT(lexer.nextToken() == Token(TokenType::OPERATOR, "+"));
        EXPECT(lexer.hasNextToken());

        EXPECT(lexer.nextToken() == Token(TokenType::NUMBER, "23"));
        EXPECT_NOT(lexer.hasNextToken());
    },

    CASE("lexing '2.3 * 1 + 4'") {
        std::istringstream input{"2.3 * 1 + 4"};
        Lexer lexer(input);
        EXPECT(lexer.hasNextToken());

        EXPECT(lexer.nextToken() == Token(TokenType::NUMBER, "2.3"));
        EXPECT(lexer.nextToken() == Token(TokenType::OPERATOR, "*"));
        EXPECT(lexer.nextToken() == Token(TokenType::NUMBER, "1"));
        EXPECT(lexer.nextToken() == Token(TokenType::OPERATOR, "+"));
        EXPECT(lexer.nextToken() == Token(TokenType::NUMBER, "4"));
        EXPECT_NOT(lexer.hasNextToken());
    },

    CASE("lexing ' (1 + 23) *   4 '") {
        std::istringstream input{" (1 + 23) *   4 "};
        Lexer lexer(input);
        EXPECT(lexer.hasNextToken());

        EXPECT(lexer.nextToken() == Token(TokenType::OPERATOR, "("));
        EXPECT(lexer.nextToken() == Token(TokenType::NUMBER, "1"));
        EXPECT(lexer.nextToken() == Token(TokenType::OPERATOR, "+"));
        EXPECT(lexer.nextToken() == Token(TokenType::NUMBER, "23"));
        EXPECT(lexer.nextToken() == Token(TokenType::OPERATOR, ")"));
        EXPECT(lexer.nextToken() == Token(TokenType::OPERATOR, "*"));
        EXPECT(lexer.nextToken() == Token(TokenType::NUMBER, "4"));

        EXPECT_NOT(lexer.hasNextToken());
    },

    CASE("lexing 'cos (3 * x ) + sin x'") {
        std::istringstream input{"cos (3 * x ) + sin x"};
        Lexer lexer(input);
        EXPECT(lexer.hasNextToken());

        EXPECT(lexer.nextToken() == Token(TokenType::IDENTIFIER, "cos"));
        EXPECT(lexer.nextToken() == Token(TokenType::OPERATOR, "("));
        EXPECT(lexer.nextToken() == Token(TokenType::NUMBER, "3"));
        EXPECT(lexer.nextToken() == Token(TokenType::OPERATOR, "*"));
        EXPECT(lexer.nextToken() == Token(TokenType::IDENTIFIER, "x"));
        EXPECT(lexer.nextToken() == Token(TokenType::OPERATOR, ")"));
        EXPECT(lexer.nextToken() == Token(TokenType::OPERATOR, "+"));
        EXPECT(lexer.nextToken() == Token(TokenType::IDENTIFIER, "sin"));
        EXPECT(lexer.nextToken() == Token(TokenType::IDENTIFIER, "x"));

        EXPECT_NOT(lexer.hasNextToken());
    },

    CASE("lexing '1 % 5") {
        std::istringstream input{"1 % 5"};
        Lexer lexer(input);
        EXPECT(lexer.hasNextToken());

        EXPECT(lexer.nextToken() == Token(TokenType::NUMBER, "1"));
        EXPECT_THROWS_AS(lexer.nextToken(), InvalidInputException);
    },

    CASE("lexing past eOF") {
        std::istringstream input{"1"};
        Lexer lexer(input);
        lexer.nextToken();
        EXPECT_NOT(lexer.hasNextToken());
        EXPECT(lexer.nextToken() == Token(TokenType::END_OF_INPUT, ""));
        EXPECT_NOT(lexer.hasNextToken());
    },

    CASE("lexing 'a\n3'") {
        std::istringstream input{"a\n3"};
        Lexer lexer(input);
        EXPECT(lexer.hasNextToken());

        EXPECT(lexer.nextToken() == Token(TokenType::IDENTIFIER, "a"));
        EXPECT(lexer.nextToken() == Token(TokenType::END_OF_LINE, ""));
        EXPECT(lexer.nextToken() == Token(TokenType::NUMBER, "3"));

        EXPECT_NOT(lexer.hasNextToken());
    }
};
