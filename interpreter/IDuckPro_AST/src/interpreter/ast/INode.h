#pragma once

#include <memory>

namespace AST {

class IVisitor;

class INode
{
public:
    virtual void visit(IVisitor& visitor) = 0;

    virtual bool isValid() const = 0;

    virtual ~INode() {}

};

typedef std::shared_ptr<INode> INodePtr;

} // AST
