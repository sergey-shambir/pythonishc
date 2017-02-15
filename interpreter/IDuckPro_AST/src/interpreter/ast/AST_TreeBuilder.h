/**
 * @brief The TreeBuilder class
 *
 * Builds abstract syntax tree from stream of tokens.
 * When error occured, builder reject all token until delimiter (semicolon or
 * end of line) scanned.
 */

#pragma once

#include "ast.h"
#include "../error/Emitter.h"

#include "../Scanner.hpp"

namespace AST {

class TreeBuilder : public IVisitor, public Error::Emitter
{
public:
    TreeBuilder();
    virtual ~TreeBuilder();

    INodePtr build(Scanner *scanner);

    void visitNode(WhileStmt &node);
    void visitNode(ActionStmt &node);
    void visitNode(StmtGroup &node);

private:
    StmtPtr visitStatement(const Token &token);
    void restoreScannerState();
    INodePtr m_root;
    Scanner *m_scanner;
};

} // namespace AST
