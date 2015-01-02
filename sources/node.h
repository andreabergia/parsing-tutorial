#ifndef NODE_H
#define NODE_H

#include <sstream>

#include "evaluation.h"

class Node
{
public:
    virtual ~Node() {}

    virtual std::string toString(bool isTopLevel) const = 0;
    virtual double eval(EvaluationContext &context) = 0;
};

class NumberNode : public Node {
public:
    NumberNode(double n) : n_(n) {}
    virtual ~NumberNode() {}

    virtual std::string toString(bool isTopLevel) const override {
        std::ostringstream oss;
        oss << n_;
        return oss.str();
    }

    virtual double eval(EvaluationContext &context) override {
        return n_;
    }

private:
    double n_;
};

class BinaryOpNode : public Node {
public:
    using evalFunc = std::function<double(double, double)>;
    using toStringFunc = std::function<std::string(std::string, std::string)>;

    BinaryOpNode(Node &left, Node &right, toStringFunc toString, evalFunc eval)
    : left_(left), right_(right), toString_(toString), eval_(eval) {}
    virtual ~BinaryOpNode() {}

    virtual std::string toString(bool isTopLevel) const override {
        std::string s = toString_(left_.toString(false), right_.toString(false));
        return isTopLevel ? s : "(" + s + ")";
    }

    virtual double eval(EvaluationContext &context) override {
        return eval_(left_.eval(context), right_.eval(context));
    }

private:
    Node &left_;
    Node &right_;
    toStringFunc toString_;
    evalFunc eval_;
};

class AdditionNode : public BinaryOpNode {
public:
    AdditionNode(Node &left, Node &right)
    : BinaryOpNode(left, right,
        [](std::string s1, std::string s2){return s1 + " + " + s2;},
        [](double v1, double v2){return v1 + v2; }) {}
    virtual ~AdditionNode() {}
};

class SubtractionNode : public BinaryOpNode {
public:
    SubtractionNode(Node &left, Node &right)
            : BinaryOpNode(left, right,
            [](std::string s1, std::string s2){return s1 + " - " + s2;},
            [](double v1, double v2){return v1 - v2; }) {}
    virtual ~SubtractionNode() {}
};

class MultiplicationNode : public BinaryOpNode {
public:
    MultiplicationNode(Node &left, Node &right)
            : BinaryOpNode(left, right,
            [](std::string s1, std::string s2){return s1 + " * " + s2;},
            [](double v1, double v2){return v1 * v2; }) {}
    virtual ~MultiplicationNode() {}
};

class DivisionNode : public BinaryOpNode {
public:
    DivisionNode(Node &left, Node &right)
            : BinaryOpNode(left, right,
            [](std::string s1, std::string s2){return s1 + " / " + s2;},
            [](double v1, double v2){return v1 / v2; }) {}
    virtual ~DivisionNode() {}
};

#endif
