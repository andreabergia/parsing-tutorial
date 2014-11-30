add_library (derivativeLib
                exceptions.h
                token.h
                lexer.h lexer.cpp)

add_executable (derivative
                main.cpp)

target_link_libraries (derivative derivativeLib)
