# Specify include dir
include_directories(
    ${PROJECT_SOURCE_DIR}/sources
    ${PROJECT_SOURCE_DIR}/libs
    )

add_executable(runTests 
                testLexer.hpp
                testParser.hpp
                testMain.cpp)

add_dependencies(runTests derivativeLib)
target_link_libraries(runTests derivativeLib)
