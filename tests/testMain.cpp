#include <vector>
#include <iterator>

#include "lest.hpp"

#include "testLexer.hpp"
#include "testParser.hpp"

template <std::size_t N>
void addTests(lest::test const (&toAdd)[N], std::vector<lest::test> &tests)
{
    std::copy(toAdd, toAdd + N, std::back_inserter(tests));
}

int main(int argc, char* argv[])
{
    std::vector<lest::test> tests;
    addTests(testLexer, tests);
    addTests(testParser, tests);
    
    return lest::run(tests, lest::texts(argv + 1, argv + argc), std::cout);
}