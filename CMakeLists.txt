project(compiler-theory-samples)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra")

aux_source_directory(lemon-1-calculator SRC_LEMON_1)
add_executable(lemon_1 ${SRC_LEMON_1})

aux_source_directory(lemon-2-interpreter SRC_LEMON_2)
add_executable(lemon_2 ${SRC_LEMON_2})

aux_source_directory(lemon-3-error-recovery SRC_LEMON_3)
add_executable(lemon_3 ${SRC_LEMON_3})

aux_source_directory(lemon-4-ast SRC_LEMON_4)
add_executable(lemon_4 ${SRC_LEMON_4})

aux_source_directory(lemon-5-minimal-structured SRC_LEMON_5)
add_executable(lemon_5 ${SRC_LEMON_5})

aux_source_directory(lemon-6-full-structured SRC_LEMON_6)
add_executable(lemon_6 ${SRC_LEMON_6})

aux_source_directory(lemon-7-functions SRC_LEMON_7)
add_executable(lemon_7 ${SRC_LEMON_7})

aux_source_directory(lemon-8-types SRC_LEMON_8)
add_executable(lemon_8 ${SRC_LEMON_8})

aux_source_directory(std-regex SRC_STL_REGEX)
add_executable(std_regex ${SRC_STL_REGEX})

aux_source_directory(std-stringview SRC_STL_STRINGVIEW)
add_executable(std_stringview ${SRC_STL_STRINGVIEW})
