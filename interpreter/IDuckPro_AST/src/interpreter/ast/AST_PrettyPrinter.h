#pragma once

#include "AST_IVisitor.h"
#include <iostream>

namespace AST {

class PrettyPrinter : public IVisitor
{
public:
    PrettyPrinter(std::ostream &stream);
    virtual ~PrettyPrinter();

    void visitNode(WhileStmt &node);
    void visitNode(ActionStmt &node);
    void visitNode(StmtGroup &node);

private:
    std::ostream &_stream;
};

} // namespace AST
