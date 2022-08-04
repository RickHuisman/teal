cmake_minimum_required(VERSION 3.22)
project(teal)

set(CMAKE_CXX_STANDARD 17)

include_directories(syntax)

add_executable(teal
        main.cpp
        syntax/lexer.h
        syntax/lexer.cpp
        syntax/ast.h
        syntax/ast.cpp
        syntax/parser.h
        syntax/parser.cpp
        compiler/compiler.h
        compiler/compiler.cpp
        compiler/value.cpp
        compiler/value.h)