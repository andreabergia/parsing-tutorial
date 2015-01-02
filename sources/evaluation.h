#ifndef EVALUATION_H
#define EVALUATION_H

#include <string>
#include <map>

// A doubleToDoubleFunction is a pointer to a function taking a double and returning a double
using doubleToDoubleFunction = double(*)(double);
using functionMap = std::map<std::string, doubleToDoubleFunction>;
using variablesMap = std::map<std::string, double>;

struct EvaluationContext {
    functionMap &functions;
    variablesMap &variables;

    EvaluationContext(functionMap &funcs, variablesMap &vars)
        : functions(funcs), variables(vars) {
    }
};

#endif
