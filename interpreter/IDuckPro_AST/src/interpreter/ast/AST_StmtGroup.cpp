#include "AST_StmtGroup.h"

#include "AST_Stmt.h"
#include "AST_IVisitor.h"

#include <assert.h>

namespace AST {

StmtGroup::StmtGroup()
{
}

StmtGroup::~StmtGroup()
{
}

void StmtGroup::visit(IVisitor &visitor)
{
    visitor.visitNode(*this);
}

bool StmtGroup::isValid() const
{
    return true;
}

void StmtGroup::pushBack(const std::shared_ptr<Stmt> &pStatement)
{
    m_group.push_back(pStatement);
}

size_t StmtGroup::getSize() const
{
    return m_group.size();
}

StmtPtr &StmtGroup::getItem(size_t index)
{
    assert(index < getSize());
    return m_group[index];
}

} // AST
