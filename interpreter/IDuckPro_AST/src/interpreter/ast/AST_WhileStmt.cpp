#include "AST_WhileStmt.h"
#include "AST_IVisitor.h"

namespace AST {

WhileStmt::WhileStmt()
{
}

WhileStmt::~WhileStmt()
{
}

void WhileStmt::visit(IVisitor &visitor)
{
    visitor.visitNode(*this);
}

bool WhileStmt::isValid() const
{
    return (! m_varName.empty() && (m_statement.get() != 0));
}

const std::string &WhileStmt::getVarName() const
{
    return m_varName;
}

StmtPtr &WhileStmt::statement()
{
    return m_statement;
}

const StmtPtr &WhileStmt::statement() const
{
    return m_statement;
}

void WhileStmt::setVarName(const std::string &name)
{
    m_varName = name;
}

} // AST
