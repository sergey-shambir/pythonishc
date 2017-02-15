#pragma once

namespace AST {

class Stmt;
class WhileStmt;
class ActionStmt;
class StmtGroup;

class IVisitor
{
public:
    virtual void visitNode(WhileStmt &node) = 0;
    virtual void visitNode(ActionStmt &node) = 0;
    virtual void visitNode(StmtGroup &node) = 0;

    virtual ~IVisitor() { }
};

} // AST
