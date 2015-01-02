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
    CASE("Recursive AdditionNode") {
        NumberNode n1(1), n2(2), n3(3);
        AdditionNode n1plus2(n1, n2);
        AdditionNode node(n1plus2, n3);
        EXPECT("1 + 2 + 3" == node.toString());
        EXPECT(approx(6) == evalNode(node));
    }
};
