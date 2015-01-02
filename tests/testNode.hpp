#include "lest.hpp"

#include "node.h"
#include "../sources/node.h"

double evalNode(Node &node) {
    functionMap functions;
    variablesMap  variables;
    EvaluationContext ec(functions, variables);

    return node.eval(ec);
}

const lest::test testNode[] = {
    CASE("NumberNode") {
        NumberNode node(0.5);
        EXPECT("0.5" == node.toString(true));
        EXPECT("0.5" == node.toString(false));
        EXPECT(approx(0.5) == evalNode(node));
    },

    CASE("AdditionNode") {
        NumberNode n1(1), n2(2);
        AdditionNode node(n1, n2);
        EXPECT("1 + 2" == node.toString(true));
        EXPECT("(1 + 2)" == node.toString(false));
        EXPECT(approx(3) == evalNode(node));
    },
    CASE("SubtractionNode") {
        NumberNode n1(1), n2(2);
        SubtractionNode node(n1, n2);
        EXPECT("1 - 2" == node.toString(true));
        EXPECT("(1 - 2)" == node.toString(false));
        EXPECT(approx(-1) == evalNode(node));
    },
    CASE("MultiplicationNode") {
        NumberNode n2(2), n3(3);
        MultiplicationNode node(n2, n3);
        EXPECT("2 * 3" == node.toString(true));
        EXPECT("(2 * 3)" == node.toString(false));
        EXPECT(approx(6) == evalNode(node));
    },
    CASE("DivisionNode") {
        NumberNode n1(1), n2(2);
        DivisionNode node(n1, n2);
        EXPECT("1 / 2" == node.toString(true));
        EXPECT("(1 / 2)" == node.toString(false));
        EXPECT(approx(0.5) == evalNode(node));
    },

    CASE("Recursive Nodes") {
        NumberNode n1(1), n2(2), n3(3);
        MultiplicationNode n2times3(n2, n3);
        AdditionNode node(n2times3, n1);
        EXPECT("(2 * 3) + 1" == node.toString(true));
        EXPECT(approx(7) == evalNode(node));
    },
    CASE("Recursive Nodes 2") {
        NumberNode n1(1), n2(2), n3(3), n4(4), n7(7);
        AdditionNode n1plus3(n1, n3);
        DivisionNode n4dividedBy2(n4, n2);
        SubtractionNode n7minusn4dividedBy2(n7, n4dividedBy2);
        DivisionNode node(n1plus3, n7minusn4dividedBy2);
        EXPECT("(1 + 3) / (7 - (4 / 2))" == node.toString(true));
        EXPECT(approx(0.8) == evalNode(node));
    },
};
