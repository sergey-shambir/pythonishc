#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#include <llvm/IR/Module.h>
#pragma clang diagnostic pop

class CCompilerBackend
{
public:
    CCompilerBackend();

    void GenerateObjectFile(llvm::Module & module,
                            bool isDebug, std::string const& outputPath);
};
