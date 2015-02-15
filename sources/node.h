#ifndef NODE_H
#define NODE_H

#include <sstream>
#include <cassert>
#include <memory>

#include "evaluation.h"
#include "exceptions.h"

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
    virtual NodePtr derivative(const std::string &argument) const = 0;
};

using NodePtr = std::shared_ptr<Node>;

class NumberNode : public Node {
public:
    NumberNode(double n) : n_(n) {}
    virtual ~NumberNode() {}

    virtual std::string toString(ToStringType toStringType) const override {
        std::ostringstream oss;
        oss << n_;
        return oss.str();
    }

    virtual NodePtr derivative(const std::string &argument) const override {
        return NodePtr(new NumberNode(0));
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

    BinaryOpNode(NodePtr left, NodePtr right, toStringFunc toString, evalFunc eval)
    : left_(left), right_(right), toString_(toString), eval_(eval) {}
    virtual ~BinaryOpNode() {}

    virtual std::string toString(ToStringType toStringType) const override {
        std::string s = toString_(
                left_->toString(ToStringType::RECURSIVE_CALL),
                right_->toString(ToStringType::RECURSIVE_CALL));
        return toStringType == ToStringType::TOP_LEVEL ? s : "(" + s + ")";
    }

    virtual double eval(EvaluationContext &context) override {
        return eval_(left_->eval(context), right_->eval(context));
    }

protected:
    NodePtr left_;
    NodePtr right_;

private:
    toStringFunc toString_;
    evalFunc eval_;
};

class AdditionNode : public BinaryOpNode {
public:
    AdditionNode(NodePtr left, NodePtr right)
    : BinaryOpNode(left, right,
        [](std::string s1, std::string s2){return s1 + " + " + s2;},
        [](double v1, double v2){return v1 + v2; }) {}
    virtual ~AdditionNode() {}

    virtual NodePtr derivative(const std::string &argument) const override {
        return NodePtr(new AdditionNode(
                left_->derivative(argument), right_->derivative(argument)));
    }
};

class SubtractionNode : public BinaryOpNode {
public:
    SubtractionNode(NodePtr left, NodePtr right)
            : BinaryOpNode(left, right,
            [](std::string s1, std::string s2){return s1 + " - " + s2;},
            [](double v1, double v2){return v1 - v2; }) {}
    virtual ~SubtractionNode() {}

    virtual NodePtr derivative(const std::string &argument) const override {
        return NodePtr(new SubtractionNode(
                left_->derivative(argument), right_->derivative(argument)));
    }
};

class MultiplicationNode : public BinaryOpNode {
public:
    MultiplicationNode(NodePtr left, NodePtr right)
            : BinaryOpNode(left, right,
            [](std::string s1, std::string s2){return s1 + " * " + s2;},
            [](double v1, double v2){return v1 * v2; }) {}
    virtual ~MultiplicationNode() {}

    virtual NodePtr derivative(const std::string &argument) const override {
        // (f g)' = f' g + f g'
        NodePtr f_g = NodePtr(new MultiplicationNode(left_->derivative(argument), right_));
        NodePtr fg_ = NodePtr(new MultiplicationNode(left_, right_->derivative(argument)));
        return NodePtr(new AdditionNode(f_g, fg_));
    }
};

class DivisionNode : public BinaryOpNode {
public:
    DivisionNode(NodePtr left, NodePtr right)
            : BinaryOpNode(left, right,
            [](std::string s1, std::string s2){return s1 + " / " + s2;},
            [](double v1, double v2){return v1 / v2; }) {}
    virtual ~DivisionNode() {}

    virtual NodePtr derivative(const std::string &argument) const override {
        // (f / g)' = (f'g - fg') / g^2
        NodePtr f_g = NodePtr(new MultiplicationNode(left_->derivative(argument), right_));
        NodePtr fg_ = NodePtr(new MultiplicationNode(left_, right_->derivative(argument)));
        NodePtr g2 = NodePtr(new MultiplicationNode(right_, right_));
        NodePtr num = NodePtr(new SubtractionNode(f_g, fg_));
        return NodePtr(new DivisionNode(num, g2));
    }
};

class VariableNode : public Node {
public:
    VariableNode(const std::string &varName) : varName_(varName) {}
    ~VariableNode() {};

    virtual std::string toString(ToStringType toStringType) const override {
        return varName_;
    }

    virtual double eval(EvaluationContext &context) override {
        return context.getVariableValue(varName_);
    }

    virtual NodePtr derivative(const std::string &argument) const override {
        if (varName_ == argument) {
            return NodePtr(new NumberNode(1));
        } else {
            return NodePtr(new NumberNode(0));
        }
    }

private:
    std::string varName_;
};

class FunctionCallNode : public Node {
public:
    FunctionCallNode(const std::string &funcName, NodePtr argumentExpression)
            : funcName_(funcName), argumentExpression_(argumentExpression) {}
    ~FunctionCallNode() {};

    virtual std::string toString(ToStringType toStringType) const override {
        std::string call = funcName_ + " " + argumentExpression_->toString(ToStringType::RECURSIVE_CALL);
        return toStringType == ToStringType::TOP_LEVEL ? call : "(" + call + ")";
    }

    virtual double eval(EvaluationContext &context) override {
        double arg = argumentExpression_->eval(context);
        return context.callFunction(funcName_, arg);
    }

    virtual NodePtr derivative(const std::string &argument) const override {
        // f(g)' = f'(g) g'
        NodePtr f_g = NodePtr(new FunctionCallNode(funcName_ + "'", argumentExpression_));
        NodePtr g_ = argumentExpression_->derivative(argument);
        return NodePtr(new MultiplicationNode(f_g, g_));
    }

private:
    std::string funcName_;
    NodePtr argumentExpression_;
};

#endif
