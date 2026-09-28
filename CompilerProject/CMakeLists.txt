cmake_minimum_required(VERSION 3.10)

project(LLVMCompiler LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Wextra"
)

# ============================================================
# Dependencies
# ============================================================

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(LLVM CONFIG REQUIRED)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")


# ============================================================
# Include directories
# ============================================================

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)


# ============================================================
# Bison
# ============================================================

BISON_TARGET(
    Parser
    ${CMAKE_SOURCE_DIR}/include/parser/parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    DEFINES_FILE
    ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp
)


# ============================================================
# Flex
# ============================================================

FLEX_TARGET(
    Lexer
    ${CMAKE_SOURCE_DIR}/include/lexer/lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
)


# Flex depends on the generated Bison parser.
ADD_FLEX_BISON_DEPENDENCY(
    Lexer
    Parser
)


# ============================================================
# Compiler executable
# ============================================================

add_executable(
    compiler

    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/AST.cpp

    ${BISON_Parser_OUTPUTS}
    ${FLEX_Lexer_OUTPUTS}
)


# ============================================================
# Generated headers
# ============================================================

target_include_directories(
    compiler
    PRIVATE

    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/include
)


# ============================================================
# LLVM
# ============================================================

target_include_directories(
    compiler
    PRIVATE

    ${LLVM_INCLUDE_DIRS}
)

llvm_map_components_to_libnames(
    LLVM_LIBS

    core
    support
    orcjit
    native
)


target_link_libraries(
    compiler
    PRIVATE
    ${LLVM_LIBS}
)


# ============================================================
# Math library
# ============================================================

if(UNIX)
    target_link_libraries(
        compiler
        PRIVATE
        m
    )
endif()


# ============================================================
# Configuration summary
# ============================================================

message(STATUS "")
message(STATUS "========================================")
message(STATUS "LLVM Compiler Configuration")
message(STATUS "========================================")
message(STATUS "  Build type:   ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  LLVM:         ${LLVM_PACKAGE_VERSION}")
message(STATUS "  Flex:         ${FLEX_VERSION}")
message(STATUS "  Bison:        ${BISON_VERSION}")
message(STATUS "========================================")
message(STATUS "")


# ============================================================
# Install
# ============================================================

install(
    TARGETS compiler
    DESTINATION bin
)
