#include <sstream>

#include "lest.hpp"
using lest::approx;

#include "parser.h"
#include "exceptions.h"

const lest::test testParser[] = {
    CASE("parsing '13'") {
        std::istringstream input{"13"};
        Parser parser(input);
        EXPECT(approx(13) == parser.evalNextExpression());
    },

    CASE("parsing '3.14'") {
        std::istringstream input{"3.14"};
        Parser parser(input);
        EXPECT(approx(3.14) == parser.evalNextExpression());
    },

    CASE("parsing '1 + 2.3'") {
        std::istringstream input{"1 + 2.3"};
        Parser parser(input);
        EXPECT(approx(3.3) == parser.evalNextExpression());
    },

    CASE("parsing '12 + 100 - 99'") {
        std::istringstream input{"12 + 100 - 99"};
        Parser parser(input);
        EXPECT(approx(13) == parser.evalNextExpression());
    },

    CASE("parsing '3 * 2 + 1'") {
        std::istringstream input{"3 * 2 + 1"};
        Parser parser(input);
        EXPECT(approx(7) == parser.evalNextExpression());
    },

    CASE("parsing '1 + 3 * 2'") {
        std::istringstream input{"1 + 3 * 2"};
        Parser parser(input);
        EXPECT(approx(7) == parser.evalNextExpression());
    },

    CASE("parsing '(54)'") {
        std::istringstream input{"(54)"};
        Parser parser(input);
        EXPECT(approx(54) == parser.evalNextExpression());
    },

    CASE("parsing ' (1 + 23) *   4 '") {
        std::istringstream input{" (1 + 23) *   4 "};
        Parser parser(input);
        EXPECT(approx(96) == parser.evalNextExpression());
    },

    CASE("parsing '((2 + 3) * (3 + 4))") {
        std::istringstream input{"((2 + 3) * (3 + 4))"};
        Parser parser(input);

        EXPECT(approx(35) == parser.evalNextExpression());
    },

    CASE("parsing '1.5 + 1 / 2") {
        std::istringstream input{"1.5 + 1 / 2"};
        Parser parser(input);

        EXPECT(approx(2) == parser.evalNextExpression());
    },

    CASE("parsing '(1") {
        std::istringstream input{"(1"};
        Parser parser(input);

        EXPECT_THROWS_AS(parser.evalNextExpression(), InvalidInputException);
    }
};
