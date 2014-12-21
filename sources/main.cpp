#include <iostream>

#include "parser.h"

int main(int argc, char *argv[])
{
    Parser parser(std::cin, std::cout);
    parser.parseProgram();

    return 0;
}
