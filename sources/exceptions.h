#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class InvalidInputException : public std::runtime_error
{
public:
    InvalidInputException(const std::string reason) : runtime_error(reason) {
    }
};

class UnknownFunctionName : public std::runtime_error
{
public:
    UnknownFunctionName(const std::string name) : runtime_error("Unknown function: " + name) {
    }
};

class UnknownVariableName : public std::runtime_error
{
public:
    UnknownVariableName(const std::string name) : runtime_error("Unknown variable: " + name) {
    }
};

#endif
