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
    }
};