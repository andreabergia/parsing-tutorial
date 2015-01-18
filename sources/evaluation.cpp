#include "evaluation.h"
#include "node.h"

static builtinFunctionMap builtinFunctions {
        {"exp", std::exp},
        {"log", std::log},
        {"sin", std::sin},
        {"cos", std::cos},
        {"tan", std::tan}
};

double EvaluationContext::getVariableValue(std::string variableName) const {
    auto it = variables_.find(variableName);
    if (it == variables_.end()) {
        throw UnknownVariableName(variableName);
    }
    return it->second;
}

double EvaluationContext::callFunction(std::string functionName, double argumentValue) const
{
    // Is it an user defined function? If so, call it.
    auto it = userFunctions_.find(functionName);
    if (it != userFunctions_.end()) {
        return callUserDefinedFunction(it->second, argumentValue);
    }

    // Is it a builtin function? If so, call it.
    auto bit = builtinFunctions.find(functionName);
    if (bit != builtinFunctions.end()) {
        return bit->second(argumentValue);
    }

    throw UnknownFunctionName(functionName);
}

double EvaluationContext::callUserDefinedFunction(UserFunctionPtr userFunction, double argumentValue) const
{
    // Create an inner evaluation context where the variable "argumentName" is set to "argumentValue"
    // and evaluate the function's expression node
    variablesMap innerScopeVariables = variables_;
    innerScopeVariables[userFunction->argumentName] = argumentValue;
    EvaluationContext innerScope(userFunctions_, innerScopeVariables);
    return userFunction->bodyNode->eval(innerScope);
}
