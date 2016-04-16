#pragma once

#include <iostream>
#include <memory>

class CInterpreter
{
public:
    CInterpreter();
    ~CInterpreter();

    void EnterLoop(std::istream &input, std::ostream &output, std::ostream &errors);

private:
    class Impl;
    std::unique_ptr<Impl> m_pImp;
};
