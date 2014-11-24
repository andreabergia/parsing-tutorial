#include <sstream>
#include "lest.hpp"
#include "lexer.h"

const lest::test specification[] = {
    CASE("parsing '1'") {
        std::istringstream input{"1"};
        Lexer lexer(input);
        EXPECT(lexer.hasNextToken());

        EXPECT(lexer.nextToken() == Token(NUMBER, "1"));

        EXPECT_NOT(lexer.hasNextToken());
    },

    CASE("parsing '1+23'") {
        std::istringstream input{"1+23"};
        Lexer lexer(input);
        EXPECT(lexer.hasNextToken());

        EXPECT(lexer.nextToken() == Token(NUMBER, "1"));
        EXPECT(lexer.hasNextToken());

        EXPECT(lexer.nextToken() == Token(OPERATOR, "+"));
        EXPECT(lexer.hasNextToken());

        EXPECT(lexer.nextToken() == Token(NUMBER, "23"));
        EXPECT_NOT(lexer.hasNextToken());
    },

    CASE("parsing '(1+23)*4'") {
        std::istringstream input{"(1+23)*4"};
        Lexer lexer(input);
        EXPECT(lexer.hasNextToken());

        EXPECT(lexer.nextToken() == Token(OPERATOR, "("));
        EXPECT(lexer.nextToken() == Token(NUMBER, "1"));
        EXPECT(lexer.nextToken() == Token(OPERATOR, "+"));
        EXPECT(lexer.nextToken() == Token(NUMBER, "23"));
        EXPECT(lexer.nextToken() == Token(OPERATOR, ")"));
        EXPECT(lexer.nextToken() == Token(OPERATOR, "*"));
        EXPECT(lexer.nextToken() == Token(NUMBER, "4"));

        EXPECT_NOT(lexer.hasNextToken());
    }
};

int main(int argc, char* argv[])
{
    return lest::run(specification, argc, argv);
}