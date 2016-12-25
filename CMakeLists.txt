cmake_minimum_required(VERSION 3.6)
project(compiler)

set(CMAKE_CXX_STANDARD 14)
include_directories(code)
include_directories(code/Checker/inc)
include_directories(code/Generator/inc)
include_directories(code/Optimizer/inc)
include_directories(code/Interpreter/inc)


set(SOURCE_FILES
        code/debug.hpp
        code/debug.cpp
        code/Checker/src/Checker.cpp
        code/Checker/inc/CheckerScanner.hpp
        code/Checker/inc/Checker.hpp
        code/Optimizer/src/Optimizer.cpp
        code/Optimizer/inc/OptimizerScanner.hpp
        code/Optimizer/inc/Optimizer.hpp
        code/Interpreter/inc/Interpreter.hpp
        code/Interpreter/src/Interpreter.cpp
        )

##### generated file ################
find_package(BISON 2.4.1)
find_package(FLEX 2.5.35)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/checker)
BISON_TARGET(BisonChecker code/Checker/gen/checker.y ${CMAKE_CURRENT_BINARY_DIR}/checker/checker.tab.y.cpp)
FLEX_TARGET(FlexChecker code/Checker/gen/checker.l  ${CMAKE_CURRENT_BINARY_DIR}/checker/checker.l.cpp)
ADD_FLEX_BISON_DEPENDENCY(FlexChecker BisonChecker)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/checker)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/optimizer)
BISON_TARGET(BisonOptimizer code/Optimizer/gen/optimizer.y ${CMAKE_CURRENT_BINARY_DIR}/optimizer/optimizer.tab.y.cpp)
FLEX_TARGET(FlexOptimizer code/Optimizer/gen/optimizer.l  ${CMAKE_CURRENT_BINARY_DIR}/optimizer/optimizer.l.cpp)
ADD_FLEX_BISON_DEPENDENCY(FlexOptimizer BisonOptimizer)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/optimizer)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(compiler
        main.cpp
        ${BISON_BisonChecker_OUTPUTS}
        ${FLEX_FlexChecker_OUTPUTS}
        ${BISON_BisonOptimizer_OUTPUTS}
        ${FLEX_FlexOptimizer_OUTPUTS}
        ${SOURCE_FILES}
        )


############## GTest ####################

# Setup testing
enable_testing()
#find_package(GTest REQUIRED)
include_directories(/home/maciek/inc)
link_directories(/home/maciek/inc)
# Add test cpp file
add_executable(runUnitTests
        code/Checker/test/CheckerTest.cpp
        code/Optimizer/test/OptimizerTest.cpp
        mainTest.cpp
        ${SOURCE_FILES}
        ${BISON_BisonChecker_OUTPUTS}
        ${FLEX_FlexChecker_OUTPUTS}
        ${BISON_BisonOptimizer_OUTPUTS}
        ${FLEX_FlexOptimizer_OUTPUTS}
        )

# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest l pthread)

add_test(
    NAME runUnitTests
    COMMAND runUnitTests
)

################################################