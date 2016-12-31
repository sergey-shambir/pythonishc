
#include "ast.h"
#include "AST_PrettyPrinter.h"

namespace AST {

PrettyPrinter::PrettyPrinter(std::ostream &stream)
    :_stream(stream)
{
}

PrettyPrinter::~PrettyPrinter()
{
}

void PrettyPrinter::visitNode(ActionStmt &node)
{
    _stream << node.getAction() << ";\n";
}

void PrettyPrinter::visitNode(StmtGroup &node)
{
    for (size_t index = 0; index < node.getSize(); ++index)
    {
        StmtPtr &item = node.getItem(index);
        item->visit(*this);
    }
    _stream << std::endl;
}

void PrettyPrinter::visitNode(WhileStmt &node)
{
    _stream << "while (" << node.getVarName() << ") ";
    node.statement()->visit(*this);
}

} // namespace AST
