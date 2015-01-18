#include "lest.hpp"

#include "node.h"
#include "../sources/node.h"

const lest::test testEvaluation[] = {
    CASE("Calling the builtin function exp 1") {
        // Call the builtin function with value 1
        NodePtr n1(new NumberNode(1));
        NodePtr functionCallNode(new FunctionCallNode("exp", n1));

        userFunctionsMap functions;
        EvaluationContext ec(functions, variablesMap());
        EXPECT(approx(M_E) == functionCallNode->eval(ec));
    },

    CASE("Calling the user defined function c0") {
        // Define a function returning always 0
        NodePtr n0(new NumberNode(0));
        UserFunctionPtr constant0(new UserFunction{"c0", "x", n0});

        // Call it with value 0
        NodePtr functionCallNode(new FunctionCallNode("c0", n0));

        userFunctionsMap functions {{constant0->name, constant0}};
        EvaluationContext ec(functions, variablesMap());
        EXPECT(approx(0.) == functionCallNode->eval(ec));
    },

    CASE("Calling the user defined function f(x) = 1 + exp x") {
        // Define the function f(x) + 1 + exp x
        NodePtr accessX(new VariableNode("x"));
        NodePtr callExp(new FunctionCallNode("exp", accessX));
        NodePtr n1(new NumberNode(1));
        NodePtr sumNode(new AdditionNode(n1, callExp));
        UserFunctionPtr f(new UserFunction{"f", "x", sumNode});

        // Call it with value 2
        NodePtr n2(new NumberNode(2));
        NodePtr functionCallNode(new FunctionCallNode("f", n2));

        userFunctionsMap functions {{f->name, f}};
        EvaluationContext ec(functions, variablesMap());
        EXPECT(approx(1 + exp(2)) == functionCallNode->eval(ec));
    },
};
