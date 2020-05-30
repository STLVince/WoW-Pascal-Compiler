cmake_minimum_required(VERSION 3.10)
project(opc)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_BUILD_TYPE Debug)

find_package(BISON)
find_package(FLEX)
find_package(LLVM CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

BISON_TARGET(parser src/pascal.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp)
FLEX_TARGET(lexer src/pascal.l
    ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

include_directories(src ${CMAKE_BINARY_DIR} ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

file(GLOB SOURCE_FILES
    "src/*.cpp"
    "src/*.hpp"
    "src/*/*.cpp"
    "src/*/*.hpp"
)

add_executable(opc
    ${BISON_parser_OUTPUTS}
    ${FLEX_lexer_OUTPUTS}
    ${SOURCE_FILES}
)

llvm_map_components_to_libnames(LLVM_LIBS support core irreader all)
target_link_libraries(opc ${LLVM_LIBS})
