#pragma once

#include <string>

namespace llvm
{
class Module;
}

class CCompilerBackend
{
public:
    CCompilerBackend();

    void GenerateObjectFile(llvm::Module & module,
                            bool isDebug, std::string const& outputPath);
};
