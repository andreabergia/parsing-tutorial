#ifndef NODE_H
#define NODE_H

#include <sstream>

#include "evaluation.h"

enum class ToStringType {
    TOP_LEVEL,
    RECURSIVE_CALL
};

class Node
{
public:
    virtual ~Node() {}

    virtual std::string toString(ToStringType toStringType) const = 0;
    virtual double eval(EvaluationContext &context) = 0;
};

class NumberNode : public Node {
public:
    NumberNode(double n) : n_(n) {}
    virtual ~NumberNode() {}

    virtual std::string toString(ToStringType toStringType) const override {
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

    virtual std::string toString(ToStringType toStringType) const override {
        std::string s = toString_(
                left_.toString(ToStringType::RECURSIVE_CALL),
                right_.toString(ToStringType::RECURSIVE_CALL));
        return toStringType == ToStringType::TOP_LEVEL ? s : "(" + s + ")";
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

class VariableNode : public Node {
public:
    VariableNode(const std::string &varName) : varName_(varName) {}
    ~VariableNode() {};

    virtual std::string toString(ToStringType toStringType) const override {
        return varName_;
    }

    virtual double eval(EvaluationContext &context) override {
        auto it = context.variables.find(varName_);
        if (it == context.variables.end()) {
            throw UnknownVariableName(varName_);
        }
        return it->second;
    }

private:
    std::string varName_;
};

class FunctionCallNode : public Node {
public:
    FunctionCallNode(const std::string &funcName, Node &argument)
            : funcName_(funcName), argument_(argument) {}
    ~FunctionCallNode() {};

    virtual std::string toString(ToStringType toStringType) const override {
        std::string call = funcName_ + " " + argument_.toString(ToStringType::RECURSIVE_CALL);
        return toStringType == ToStringType::TOP_LEVEL ? call : "(" + call + ")";
    }

    virtual double eval(EvaluationContext &context) override {
        double arg = argument_.eval(context);

        auto it = context.functions.find(funcName_);
        if (it == context.functions.end()) {
            throw UnknownFunctionName(funcName_);
        }
        return it->second(arg);
    }

private:
    std::string funcName_;
    Node &argument_;
};

#endif
