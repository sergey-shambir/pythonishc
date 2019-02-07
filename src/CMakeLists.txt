project(pythonish-compiler)
cmake_minimum_required(VERSION 2.8)

# Exports LLVM_INCLUDEDIR, LLVM_LIBDIR, LLVM_LIBS, LLVM_SYSTEM_LIBS
include(cmake/CustomFindLLVM.cmake)

# Add header & libs search directories
include_directories(${LLVM_INCLUDEDIR})
include_directories(${PROJECT_SOURCE_DIR}/includes)
link_directories(${LLVM_LIBDIR})
message(${PROJECT_SOURCE_DIR}/includes)

# LLVM requires following macros defined.
add_definitions(-D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS)

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")
endif(UNIX)

find_package(Boost 1.62 COMPONENTS program_options REQUIRED)
include_directories( ${Boost_INCLUDE_DIR} )

file(GLOB SRC_pythonishc "pythonishc/*.cpp" "pythonishc/*.h")
add_executable(pythonishc ${SRC_pythonishc})
target_link_libraries(pythonishc ${LLVM_LIBS} ${LLVM_SYSTEM_LIBS} ${Boost_LIBRARIES})
