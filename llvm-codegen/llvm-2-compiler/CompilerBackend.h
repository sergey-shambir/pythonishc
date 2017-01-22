#pragma once

#include "begin_llvm.h"
#include <llvm/IR/Module.h>
#include "end_llvm.h"

class CCompilerBackend
{
public:
    CCompilerBackend();

    void GenerateObjectFile(llvm::Module & module,
                            bool isDebug, std::string const& outputPath);
};
