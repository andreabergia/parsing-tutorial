#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class InvalidInputException : public std::runtime_error
{
public:
    InvalidInputException(const std::string reason) : runtime_error(reason)
    {
    }
};

#endif