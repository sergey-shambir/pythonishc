
// Starts block of code where warnings in LLVM libraries turned off.
// Block should be ended with `#include "end_llvm.h"`

#if defined(__clang__) // For LLVM/Clang compiler
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__) // For G++/MinGW compiler
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#elif defined(_MSC_VER) // For MSVC compiler
    #pragma warning(push, 3)
#endif
