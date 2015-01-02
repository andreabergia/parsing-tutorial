#ifndef NODE_H
#define NODE_H

#include <sstream>

#include "evaluation.h"

class Node
{
public:
    virtual ~Node() {}

    virtual std::string toString() const = 0;
    virtual double eval(EvaluationContext &context) = 0;
};

class NumberNode : public Node {
public:
    NumberNode(double n) : n_(n) {}
    virtual ~NumberNode() {}

    virtual std::string toString() const override {
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

class AdditionNode : public Node {
public:
    AdditionNode(Node &left, Node &right) : left_(left), right_(right) {}
    virtual ~AdditionNode() {}

    virtual std::string toString() const override {
        return left_.toString() + " + " + right_.toString();
    }

    virtual double eval(EvaluationContext &context) override {
        return left_.eval(context) + right_.eval(context);
    }

private:
    Node &left_;
    Node &right_;
};

#endif
