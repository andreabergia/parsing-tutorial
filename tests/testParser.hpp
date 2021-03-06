#include <sstream>
#include <cmath>

#include "lest.hpp"
using lest::approx;

#include "parser.h"
#include "exceptions.h"
#include "utility.h"

double parseExpression(std::string expression)
{
    std::istringstream input{expression};
    Parser parser(input);
    return parser.evalNode(parser.getNextExpressionNode());
}

std::string parseProgramOutput(std::string program)
{
    std::ostringstream output;
    std::istringstream input{program};
    Parser parser(input, output);
    parser.parseProgram();

    // Normalize EOL to unix style
    return replaceAll(output.str(), "\r\n", "\n");
}

const lest::test testParser[] = {
    // Expressions

    CASE("parsing '13'") {
        EXPECT(approx(13) == parseExpression("13"));
    },

    CASE("parsing '3.14'") {
        EXPECT(approx(3.14) == parseExpression("3.14"));
    },

    CASE("parsing '1 + 2.3'") {
        EXPECT(approx(3.3) == parseExpression("1 + 2.3"));
    },

    CASE("parsing '12 + 100 - 99'") {
        EXPECT(approx(13) == parseExpression("12 + 100 - 99"));
    },

    CASE("parsing '3 * 2 + 1'") {
        EXPECT(approx(7) == parseExpression("3 * 2 + 1"));
    },

    CASE("parsing '1 + 3 * 2'") {
        EXPECT(approx(7) == parseExpression("1 + 3 * 2"));
    },

    CASE("parsing '(54)'") {
        EXPECT(approx(54) == parseExpression("(54)"));
    },

    CASE("parsing ' (1 + 23) *   4 '") {
        EXPECT(approx(96) == parseExpression(" (1 + 23) *   4 "));
    },

    CASE("parsing '((2 + 3) * (3 + 4))") {
        EXPECT(approx(35) == parseExpression("((2 + 3) * (3 + 4))"));
    },

    CASE("parsing '1.5 + 1 / 2") {
        EXPECT(approx(2) == parseExpression("1.5 + 1 / 2"));
    },

    CASE("parsing 'exp (1)") {
        EXPECT(approx(M_E) == parseExpression("exp (1)"));
    },

    CASE("parsing 'foo(1)") {
        EXPECT_THROWS_AS(parseExpression("foo(1)"), UnknownFunctionName);
    },

    CASE("parsing 'sin(2 * 3.141592653)") {
        EXPECT(approx(0.) == parseExpression("sin(2 * 3.141592653)"));
    },

    CASE("parsing '(1") {
        EXPECT_THROWS_AS(parseExpression("(1"), InvalidInputException);
    },

    CASE("parsing 'exp") {
        EXPECT_THROWS_AS(parseExpression("exp"), UnknownVariableName);
    },

    CASE("parsing 'e + 1'") {
        EXPECT(approx(M_E + 1) == parseExpression("e + 1"));
    },

    CASE("parsing 'sin(pi)'") {
        EXPECT(approx(0) == parseExpression("sin(pi)"));
    },

    CASE("parsing 'zz'") {
        EXPECT_THROWS_AS(parseExpression("zz"), UnknownVariableName);
    },

    // Programs
    CASE("parsing program 1 should print 1 EOL") {
        EXPECT("1\n" == parseProgramOutput("1"));
    },

    CASE("parsing program 1 + 2 EOL 1 + 3 should print 3 EOL 4 EOL") {
        EXPECT("3\n4\n" == parseProgramOutput("1 + 2\n1 + 3\n"));
    },

    CASE("parsing program EOL 3 EOL EOL 4 should print 3 EOL 4 EOL") {
        EXPECT("3\n4\n" == parseProgramOutput("\n3\n\n4"));
    },

    // Programs with variables
    CASE("parsing program a = 3 EOL a * 7 should print 21 EOL") {
        EXPECT("21\n" == parseProgramOutput("a = 3\na * 7\n"));
    },

    CASE("parsing program a = 3 EOL b = a * 7 EOL a = b + 1 EOL a should print 22 EOL") {
        EXPECT("22\n" == parseProgramOutput("a = 3\nb = a * 7\na = b + 1\na"));
    },

    // Programs with functions
    CASE("parsing program def f x = 1 + sin(x) EOL f(0) EOL should print 1 EOL") {
        EXPECT("1\n" == parseProgramOutput("def f x = 1 + sin(x)\nf(0)\n"));
    },
    CASE("parsing program def double x = x * 2 EOL def square y = y * y EOL square(double(1)) EOL should print 4 EOL") {
        EXPECT("4\n" == parseProgramOutput("def double x = x * 2\ndef square y = y * y\nsquare(double(1))\n"));
    },

    // Program with derivatives
    CASE("parsing program def f x = 2 * x - sin(x) EOL der f EOL should print ((0 * x) + (2 * 1)) - ((sin' x) * 1)") {
        EXPECT("((0 * x) + (2 * 1)) - ((sin' x) * 1)\n" == parseProgramOutput("def f x = 2 * x - sin(x)\nder f\n"));
    }
};
