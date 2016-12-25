cmake_minimum_required(VERSION 3.6)
project(compiler)

set(CMAKE_CXX_STANDARD 14)
include_directories(code)
include_directories(code/Checker/inc)
include_directories(code/Generator/inc)
include_directories(code/Optimizer/inc)


set(SOURCE_FILES
        code/debug.hpp
        code/debug.cpp
        code/Checker/src/Checker.cpp
        code/Checker/inc/Scanner.hpp
        code/Checker/inc/Checker.hpp
        )

##### generated file ################
find_package(BISON 2.4.1)
find_package(FLEX 2.5.35)

BISON_TARGET(MyParser code/Checker/gen/checker.y ${CMAKE_CURRENT_BINARY_DIR}/checker.tab.y.cpp)
FLEX_TARGET(MyScanner code/Checker/gen/checker.l  ${CMAKE_CURRENT_BINARY_DIR}/checker.l.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(compiler
        main.cpp
        ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyScanner_OUTPUTS}
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
        mainTest.cpp
        ${SOURCE_FILES}
        ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyScanner_OUTPUTS}
        )

# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest l pthread)

add_test(
    NAME runUnitTests
    COMMAND runUnitTests
)

################################################