#ifndef EVALUATION_H
#define EVALUATION_H

#include <string>
#include <map>
#include <memory>
#include <cmath>

// Forward declarations
class Node;
using NodePtr = std::shared_ptr<Node>;

// A builtinFunction is a pointer to a function taking a double and returning a double
using builtinFunction = double(*)(double);
using builtinFunctionMap = std::map<std::string, builtinFunction>;

// An user-defined function has three things: its name, its arguments and the node representing the body
struct UserFunction {
    std::string name;
    std::string argumentName;
    NodePtr bodyNode;
};
using UserFunctionPtr = std::shared_ptr<UserFunction>;

using userFunctionsMap = std::map<std::string, UserFunctionPtr>;
using variablesMap = std::map<std::string, double>;

class EvaluationContext {
public:

    EvaluationContext(userFunctionsMap userFunctions, variablesMap variables)
        : userFunctions_(userFunctions), variables_(variables) {}

    double getVariableValue(std::string variableName) const;
    double callFunction(std::string functionName, double argument) const;

private:
    userFunctionsMap userFunctions_;
    variablesMap variables_;

    double callUserDefinedFunction(UserFunctionPtr userFunction, double argumentValue) const;
};

#endif
