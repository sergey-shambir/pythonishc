#pragma once

#include <iostream>
#include <memory>

class CCompilerDriver
{
public:
    CCompilerDriver(std::ostream &errors);
    ~CCompilerDriver();

    void StartDebugTrace();

    /**
     * @param inputPath - input file path
     * @param outputPath - output file path
     */
    bool Compile(const std::string &inputPath, const std::string &outputPath);

private:
    class Impl;
    std::unique_ptr<Impl> m_pImpl;
};
