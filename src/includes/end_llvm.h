
// Ends block of code started with `#include "begin_llvm.h"`

#if defined(__clang__) // For LLVM/Clang compiler
    #pragma clang diagnostic pop
#elif defined(__GNUC__) // For G++/MinGW compiler
    #pragma GCC diagnostic pop
#elif defined(_MSC_VER) // For MSVC compiler
    #pragma warning(pop)
#endif
