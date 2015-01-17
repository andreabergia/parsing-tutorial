#include "lest.hpp"

#include "node.h"
#include "../sources/node.h"

double evalNode(NodePtr node) {
    functionMap functions {{"sin", std::sin}, {"cos", std::cos}, {"exp", std::exp}, {"log", std::log}};
    variablesMap  variables {{"a", 0.8}, {"b", 1.2}};
    EvaluationContext ec(functions, variables);
    return node->eval(ec);
}

const lest::test testNode[] = {
    CASE("NumberNode") {
        NodePtr node(new NumberNode(0.5));
        EXPECT("0.5" == node->toString(ToStringType::TOP_LEVEL));
        EXPECT("0.5" == node->toString(ToStringType::RECURSIVE_CALL));
        EXPECT(approx(0.5) == evalNode(node));
    },

    CASE("AdditionNode") {
        NodePtr n1(new NumberNode(1)), n2(new NumberNode(2));
        NodePtr node(new AdditionNode(n1, n2));
        EXPECT("1 + 2" == node->toString(ToStringType::TOP_LEVEL));
        EXPECT("(1 + 2)" == node->toString(ToStringType::RECURSIVE_CALL));
        EXPECT(approx(3) == evalNode(node));
    },
    CASE("SubtractionNode") {
        NodePtr n1(new NumberNode(1)), n2(new NumberNode(2));
        NodePtr node(new SubtractionNode(n1, n2));
        EXPECT("1 - 2" == node->toString(ToStringType::TOP_LEVEL));
        EXPECT("(1 - 2)" == node->toString(ToStringType::RECURSIVE_CALL));
        EXPECT(approx(-1) == evalNode(node));
    },
    CASE("MultiplicationNode") {
        NodePtr n2(new NumberNode(2)), n3(new NumberNode(3));
        NodePtr node(new MultiplicationNode(n2, n3));
        EXPECT("2 * 3" == node->toString(ToStringType::TOP_LEVEL));
        EXPECT("(2 * 3)" == node->toString(ToStringType::RECURSIVE_CALL));
        EXPECT(approx(6) == evalNode(node));
    },
    CASE("DivisionNode") {
        NodePtr n1(new NumberNode(1)), n2(new NumberNode(2));
        NodePtr node(new DivisionNode(n1, n2));
        EXPECT("1 / 2" == node->toString(ToStringType::TOP_LEVEL));
        EXPECT("(1 / 2)" == node->toString(ToStringType::RECURSIVE_CALL));
        EXPECT(approx(0.5) == evalNode(node));
    },

    CASE("Recursive Nodes") {
        NodePtr n1(new NumberNode(1)), n2(new NumberNode(2)), n3(new NumberNode(3));
        NodePtr n2times3(new MultiplicationNode(n2, n3));
        NodePtr node(new AdditionNode(n2times3, n1));
        EXPECT("(2 * 3) + 1" == node->toString(ToStringType::TOP_LEVEL));
        EXPECT(approx(7) == evalNode(node));
    },
    CASE("Recursive Nodes 2") {
        NodePtr n1(new NumberNode(1)), n2(new NumberNode(2)), n3(new NumberNode(3)), n4(new NumberNode(4)), n7(new NumberNode(7));
        NodePtr n1plus3(new AdditionNode(n1, n3));
        NodePtr n4dividedBy2(new DivisionNode(n4, n2));
        NodePtr n7minusn4dividedBy2(new SubtractionNode(n7, n4dividedBy2));
        NodePtr node(new DivisionNode(n1plus3, n7minusn4dividedBy2));
        EXPECT("(1 + 3) / (7 - (4 / 2))" == node->toString(ToStringType::TOP_LEVEL));
        EXPECT(approx(0.8) == evalNode(node));
    },

    CASE("Variable node") {
        NodePtr node(new VariableNode("a"));
        EXPECT("a" == node->toString(ToStringType::TOP_LEVEL));
        EXPECT("a" == node->toString(ToStringType::RECURSIVE_CALL));
        EXPECT(approx(0.8) == evalNode(node));
    },
    CASE("Unknown variable name") {
        NodePtr node(new VariableNode("zz"));
        EXPECT("zz" == node->toString(ToStringType::TOP_LEVEL));
        EXPECT("zz" == node->toString(ToStringType::RECURSIVE_CALL));
        EXPECT_THROWS_AS(evalNode(node), UnknownVariableName);
    },

    CASE("Function node") {
        NodePtr n0(new NumberNode(0));
        NodePtr node(new FunctionCallNode("sin", n0));
        EXPECT("sin 0" == node->toString(ToStringType::TOP_LEVEL));
        EXPECT("(sin 0)" == node->toString(ToStringType::RECURSIVE_CALL));
        EXPECT(approx(0.) == evalNode(node));
    },

    CASE("Unknown function name") {
        NodePtr n0(new NumberNode(0));
        NodePtr node(new FunctionCallNode("foo", n0));
        EXPECT("foo 0" == node->toString(ToStringType::TOP_LEVEL));
        EXPECT("(foo 0)" == node->toString(ToStringType::RECURSIVE_CALL));
        EXPECT_THROWS_AS(evalNode(node), UnknownFunctionName);
    },
    CASE("Recursive function call") {
        NodePtr n0(new NumberNode(0));
        NodePtr nSin(new FunctionCallNode("sin", n0));
        NodePtr node(new FunctionCallNode("exp", nSin));
        EXPECT("exp (sin 0)" == node->toString(ToStringType::TOP_LEVEL));
        EXPECT("(exp (sin 0))" == node->toString(ToStringType::RECURSIVE_CALL));
        EXPECT(approx(1.) == evalNode(node));
    }
};
