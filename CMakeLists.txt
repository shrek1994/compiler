cmake_minimum_required(VERSION 3.6)
project(compiler)

set(CMAKE_CXX_STANDARD 14)
include_directories(code)
include_directories(code/inc)
include_directories(code/Checker/inc)
include_directories(code/Generator/inc)
include_directories(code/Optimizer/inc)
include_directories(code/Interpreter/inc)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/checker)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/optimizer)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generator)


##### generated file ################
find_package(BISON 2.4.1)
find_package(FLEX 2.5.35)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/checker)
BISON_TARGET(BisonChecker code/Checker/gen/checker.y ${CMAKE_CURRENT_BINARY_DIR}/checker/checker.tab.y.cpp)
FLEX_TARGET(FlexChecker code/Checker/gen/checker.l  ${CMAKE_CURRENT_BINARY_DIR}/checker/checker.l.cpp)
ADD_FLEX_BISON_DEPENDENCY(FlexChecker BisonChecker)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/optimizer)
BISON_TARGET(BisonOptimizer code/Optimizer/gen/optimizer.y ${CMAKE_CURRENT_BINARY_DIR}/optimizer/optimizer.tab.y.cpp)
FLEX_TARGET(FlexOptimizer code/Optimizer/gen/optimizer.l  ${CMAKE_CURRENT_BINARY_DIR}/optimizer/optimizer.l.cpp)
ADD_FLEX_BISON_DEPENDENCY(FlexOptimizer BisonOptimizer)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generator)
BISON_TARGET(BisonGenerator code/Generator/gen/compiler.y ${CMAKE_CURRENT_BINARY_DIR}/generator/compiler.tab.y.cpp)
FLEX_TARGET(FlexGenerator code/Generator/gen/compiler.l  ${CMAKE_CURRENT_BINARY_DIR}/generator/compiler.l.cpp)
ADD_FLEX_BISON_DEPENDENCY(FlexOptimizer BisonOptimizer)

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
        code/Generator/src/Generator.cpp
        code/Generator/src/Driver.cpp
        code/Generator/inc/Generator.hpp
        code/Generator/inc/Driver.hpp
        code/Generator/inc/GeneratorScanner.hpp
        code/inc/Variable.hpp
        ${BISON_BisonChecker_OUTPUTS}
        ${FLEX_FlexChecker_OUTPUTS}
        ${BISON_BisonOptimizer_OUTPUTS}
        ${FLEX_FlexOptimizer_OUTPUTS}
        ${BISON_BisonGenerator_OUTPUTS}
        ${FLEX_FlexGenerator_OUTPUTS}
        code/inc/Condition.hpp code/inc/Scanner.hpp code/compiler.cpp code/compiler.hpp)

add_executable(compiler
        main.cpp
        ${SOURCE_FILES}
        )


############## GTest ####################

set(TEST_FILES
        code/Checker/test/CheckerTest.cpp
        code/Optimizer/test/OptimizerTest.cpp
        code/test/AssignTest.cpp
        code/test/BasicTest.cpp
        code/test/CompilerTest.cpp
        code/test/CompilerTest.hpp
        code/test/DriverTest.cpp
        code/test/IfElseTest.cpp
        code/test/ReadWriteTest.cpp
        )

# Setup testing
enable_testing()
#find_package(GTest REQUIRED)
include_directories(~/inc)
link_directories(~/inc)
# Add test cpp file
add_executable(runUnitTests
        mainTest.cpp
        ${SOURCE_FILES}
        ${TEST_FILES}
        )

# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest pthread)

add_test(
    NAME runUnitTests
    COMMAND runUnitTests
)

################################################