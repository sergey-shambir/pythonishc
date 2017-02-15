#pragma once

#include "INode.h"

namespace AST {

class Stmt : public INode
{
public:
    Stmt();
    virtual ~Stmt();
};

typedef std::shared_ptr<Stmt> StmtPtr;

} // AST
