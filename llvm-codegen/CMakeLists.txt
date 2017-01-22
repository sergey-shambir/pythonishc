project(compiler-theory-samples)
cmake_minimum_required(VERSION 2.8)

# Exports LLVM_INCLUDEDIR, LLVM_LIBDIR, LLVM_LIBS, LLVM_SYSTEM_LIBS
include(cmake/CustomFindLLVM.cmake)

# Add header & libs search directories
include_directories(${LLVM_INCLUDEDIR})
link_directories(${LLVM_LIBDIR})

# LLVM requires following macros defined.
add_definitions(-D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS)

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")
endif(UNIX)

macro(add_sample NAME)
    file(GLOB SRC_${NAME} "${NAME}/*.cpp" "${NAME}/*.h")
    add_executable(${NAME} ${SRC_${NAME}})
    target_link_libraries(${NAME} ${LLVM_LIBS} ${LLVM_SYSTEM_LIBS})
endmacro(add_sample)

add_sample(llvm-1-ir-translator)
add_sample(llvm-2-compiler)
add_sample(llvm-3-structured)
add_sample(llvm-4-types)
