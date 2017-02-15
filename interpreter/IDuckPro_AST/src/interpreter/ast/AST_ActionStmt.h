#pragma once

#include "AST_Stmt.h"
#include <string>

namespace AST {

class ActionStmt : public Stmt
{
public:
    ActionStmt();
    virtual ~ActionStmt();

    void visit(IVisitor &visitor);

    bool isValid() const;

    void setAction(const std::string &action);

    const std::string &getAction() const;

private:
    std::string m_action;
};

} // AST
