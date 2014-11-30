#include <sstream>

#include "lest.hpp"

#include "parser.h"
#include "exceptions.h"

const lest::test testParser[] = {
    CASE("parsing '13'") {
        std::istringstream input{"13"};
        Parser parser(input);
        EXPECT(13 == parser.evalNextExpression());
    },

    CASE("parsing '1 + 23'") {
        std::istringstream input{"1 + 23"};
        Parser parser(input);
        EXPECT(24 == parser.evalNextExpression());
    },

    CASE("parsing '3 * 2 + 1'") {
        std::istringstream input{ "3 * 2 + 1" };
        Parser parser(input);
        EXPECT(7 == parser.evalNextExpression());
    },

    CASE("parsing ' (1 + 23) *   4 '") {
        std::istringstream input{" (1 + 23) *   4 "};
        Parser parser(input);
        EXPECT(96 == parser.evalNextExpression());
    },

    CASE("parsing '((2 + 3) * (3 + 4))") {
        std::istringstream input{"((2 + 3) * (3 + 4))"};
        Lexer lexer(input);
        Parser parser(input);

        EXPECT(42 == parser.evalNextExpression());
    }
};