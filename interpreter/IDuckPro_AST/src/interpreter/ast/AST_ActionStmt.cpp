#include "AST_ActionStmt.h"
#include "AST_IVisitor.h"

namespace AST {

ActionStmt::ActionStmt()
{
}

ActionStmt::~ActionStmt()
{
}

void ActionStmt::visit(IVisitor &visitor)
{
    visitor.visitNode(*this);
}

bool ActionStmt::isValid() const
{
    return (! m_action.empty());
}

void ActionStmt::setAction(const std::string &action)
{
    m_action = action;
}

const std::string &ActionStmt::getAction() const
{
    return m_action;
}

} // AST
