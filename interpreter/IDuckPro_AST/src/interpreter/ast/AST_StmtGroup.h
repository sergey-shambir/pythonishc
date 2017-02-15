#pragma once

#include <memory>
#include <vector>
#include "AST_Stmt.h"

namespace AST {

class StmtGroup : public INode
{
public:
    StmtGroup();
    virtual ~StmtGroup();

    void visit(IVisitor &visitor);

    bool isValid() const;

    void pushBack(const std::shared_ptr<Stmt> &pStatement);
    size_t getSize() const;
    StmtPtr &getItem(size_t index);

private:
    std::vector<StmtPtr> m_group;
};

} // AST
