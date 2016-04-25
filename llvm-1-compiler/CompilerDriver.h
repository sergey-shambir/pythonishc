#pragma once

#include <iostream>
#include <memory>

class CCompilerDriver
{
public:
    CCompilerDriver(std::ostream &errors);
    ~CCompilerDriver();

    void StartDebugTrace();
    bool Compile(std::istream &input, std::ostream &output);

private:
    class Impl;
    std::unique_ptr<Impl> m_pImpl;
};
