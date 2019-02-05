# Searches LLVM 3.9 includes and static libraries

find_program(
    LLVM_CONFIG_EXECUTABLE
    llvm-config-3.9
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
