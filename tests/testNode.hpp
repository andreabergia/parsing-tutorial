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
        EXPECT("0.5" == node.toString());
        EXPECT(approx(0.5) == evalNode(node));
    },

    CASE("AdditionNode") {
        NumberNode n1(1), n2(2);
        AdditionNode node(n1, n2);
        EXPECT("1 + 2" == node.toString());
        EXPECT(approx(3) == evalNode(node));
    },
    CASE("SubtractionNode") {
        NumberNode n1(1), n2(2);
        SubtractionNode node(n1, n2);
        EXPECT("1 - 2" == node.toString());
        EXPECT(approx(-1) == evalNode(node));
    },
    CASE("MultiplicationNode") {
        NumberNode n2(2), n3(3);
        MultiplicationNode node(n2, n3);
        EXPECT("2 * 3" == node.toString());
        EXPECT(approx(6) == evalNode(node));
    },
    CASE("DivisionNode") {
        NumberNode n1(1), n2(2);
        DivisionNode node(n1, n2);
        EXPECT("1 / 2" == node.toString());
        EXPECT(approx(0.5) == evalNode(node));
    },

    CASE("Recursive Nodes") {
        NumberNode n1(1), n2(2), n3(3);
        MultiplicationNode n2times3(n2, n3);
        AdditionNode node(n2times3, n1);
        EXPECT("2 * 3 + 1" == node.toString());
        EXPECT(approx(7) == evalNode(node));
    },
};
