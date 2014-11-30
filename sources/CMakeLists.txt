add_library (derivativeLib
                exceptions.h
                token.h
                lexer.h lexer.cpp
                parser.h parser.cpp)

add_executable (derivative
                main.cpp)

target_link_libraries (derivative derivativeLib)
