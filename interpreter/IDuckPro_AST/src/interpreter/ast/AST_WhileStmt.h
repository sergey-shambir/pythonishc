#pragma once

#include <string>
#include "AST_Stmt.h"

namespace AST {

class WhileStmt : public Stmt
{
public:
    WhileStmt();
    virtual ~WhileStmt();

    void visit(IVisitor &visitor);

    bool isValid() const;

    const std::string &getVarName() const;
    void setVarName(const std::string &name);

    StmtPtr &statement();
    const StmtPtr &statement() const;

private:
    std::string m_varName;
    StmtPtr m_statement;
};

} // AST
