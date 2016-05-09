project(compiler-theory-samples)
cmake_minimum_required(VERSION 2.8)

find_program(
    LLVM_CONFIG_EXECUTABLE
    llvm-config-3.8
  )
if (LLVM_CONFIG_EXECUTABLE)
    message(STATUS "Found LLVM configuration tool: ${LLVM_CONFIG_EXECUTABLE}")

    # Get LLVM library dir
    execute_process(
        COMMAND ${LLVM_CONFIG_EXECUTABLE} --libdir
        OUTPUT_VARIABLE LLVM_LIBDIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )

    # Get LLVM includepath dir
    execute_process(
        COMMAND ${LLVM_CONFIG_EXECUTABLE} --includedir
        OUTPUT_VARIABLE LLVM_INCLUDEDIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )

  # Get LLVM includepath dir
  execute_process(
      COMMAND ${LLVM_CONFIG_EXECUTABLE} --libdir
      OUTPUT_VARIABLE LLVM_LIBDIR
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )

  # Get LLVM system libraries
  execute_process(
      COMMAND ${LLVM_CONFIG_EXECUTABLE} --system-libs
      OUTPUT_VARIABLE LLVM_SYSTEM_LIBS
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  execute_process(
      COMMAND ${LLVM_CONFIG_EXECUTABLE} --libs
      OUTPUT_VARIABLE LLVM_LIBS
      OUTPUT_STRIP_TRAILING_WHITESPACE

    )
  string(REGEX REPLACE "-l(\\w+)" "\\1" LLVM_SYSTEM_LIBS ${LLVM_SYSTEM_LIBS})
  string(REGEX REPLACE "-l(\\w+)" "lib\\1.a" LLVM_LIBS ${LLVM_LIBS})
endif()

include_directories(${LLVM_INCLUDEDIR})
link_directories(${LLVM_LIBDIR})

add_definitions(-D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")

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

aux_source_directory(lemon-9-final SRC_LEMON_9)
add_executable(lemon_9 ${SRC_LEMON_9})

aux_source_directory(llvm-1-ir-translator SRC_LLVM_1)
add_executable(llvm_1 ${SRC_LLVM_1})
target_link_libraries(llvm_1 ${LLVM_LIBS} ${LLVM_SYSTEM_LIBS})

aux_source_directory(llvm-2-compiler SRC_LLVM_2)
add_executable(llvm_2 ${SRC_LLVM_2})
target_link_libraries(llvm_2 ${LLVM_LIBS} ${LLVM_SYSTEM_LIBS})

aux_source_directory(llvm-3-structured SRC_LLVM_3)
add_executable(llvm_3 ${SRC_LLVM_3})
target_link_libraries(llvm_3 ${LLVM_LIBS} ${LLVM_SYSTEM_LIBS})

aux_source_directory(std-regex SRC_STL_REGEX)
add_executable(std_regex ${SRC_STL_REGEX})

aux_source_directory(bison-flex-checker SRC_BISON_FLEX_CHECKER)
add_executable(bison_flex_checker ${SRC_BISON_FLEX_CHECKER})
