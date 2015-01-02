add_library (derivativeLib
                exceptions.h
                token.h
                lexer.h lexer.cpp
                evaluation.h
                node.h
                parser.h parser.cpp
                utility.h utility.cpp)

add_executable (derivative
                main.cpp)

target_link_libraries (derivative derivativeLib)
