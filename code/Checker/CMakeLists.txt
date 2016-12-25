project(Checker)

find_package(BISON 2.4.1)
find_package(FLEX 2.5.35)

BISON_TARGET(Bison code/Checker/gen/checker.y ${CMAKE_CURRENT_BINARY_DIR}/checker.tab.y.cpp)
FLEX_TARGET(Flex code/Checker/gen/checker.l  ${CMAKE_CURRENT_BINARY_DIR}/checker.l.cpp)
ADD_FLEX_BISON_DEPENDENCY(Bison Flex)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})