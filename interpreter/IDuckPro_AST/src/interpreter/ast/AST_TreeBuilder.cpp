
#include "AST_TreeBuilder.h"
#include "../ASTInterpreter.h"

namespace AST {

TreeBuilder::TreeBuilder()
{
}

TreeBuilder::~TreeBuilder()
{
}

INodePtr TreeBuilder::build(Scanner *scanner)
{
    m_scanner = scanner;
    m_root = INodePtr(new StmtGroup);
    m_root->visit(*this);

    return m_root;
}

/**
 * looks for delimiter after action statement
 */
void TreeBuilder::visitNode(ActionStmt &/*node*/)
{
    Token token = m_scanner->scan();
    if ((token.getType() != Token::Type_SEMICOLON)
            && (token.getType() != Token::Type_EOF))
    {
        std::string message = "Expected semicolon, got " + token.classify();
        notify(Error::ErrorInfo(message, token));
        restoreScannerState();
    }
}

void TreeBuilder::visitNode(StmtGroup &node)
{
    for (Token token = m_scanner->scan();
         token.getType() != Token::Type_EOF;
         token = m_scanner->scan())
    {
        if (token.getType() == Token::Type_IDENTIFIER)
        {
            StmtPtr stmt = visitStatement(token);
            if (stmt->isValid())
            {
                node.pushBack(stmt);
            }
        }
        else
        {
            std::string message = "Unexpected token: " + token.classify();
            notify(Error::ErrorInfo(message, token));
            if (token.getType() != Token::Type_SEMICOLON)
                restoreScannerState();
        }
    }
}

/**
 * @brief TreeBuilder::visitNode
 * @param node
 *
 * Looks for variable name and statement, checks syntax correctness
 */
void TreeBuilder::visitNode(WhileStmt &node)
{
    Token token = m_scanner->scan();
    if (token.getType() != Token::Type_LBRACKET)
    {
        std::string message = "Expected '(' in while condition, got "
                + token.classify();
        notify(Error::ErrorInfo(message, token));
        if (token.getType() != Token::Type_SEMICOLON)
            restoreScannerState();
        return;
    }

    token = m_scanner->scan();
    if (token.getType() != Token::Type_IDENTIFIER)
    {
        std::string message = "Expected variable name in while condition, got "
                + token.classify();
        notify(Error::ErrorInfo(message, token));
        if (token.getType() != Token::Type_SEMICOLON)
            restoreScannerState();
        return;
    }

    if (!ASTInterpreter::isVariableSupported(token.getValue()))
    {
        notify(Error::ErrorInfo("Variable not supported", token));
        restoreScannerState();
        return;
    }

    node.setVarName(token.getValue());

    token = m_scanner->scan();
    if (token.getType() != Token::Type_RBRACKET)
    {
        std::string message = "Expected ')' in while condition, got "
                + token.classify();
        notify(Error::ErrorInfo(message, token));
        if (token.getType() != Token::Type_SEMICOLON)
            restoreScannerState();
        return;
    }

    token = m_scanner->scan();
    if (token.getType() != Token::Type_IDENTIFIER)
    {
        std::string message = "Expected statement after while, got "
                + token.classify();
        notify(Error::ErrorInfo(message, token));
        if (token.getType() != Token::Type_SEMICOLON)
            restoreScannerState();
        return;
    }

    StmtPtr stmt = visitStatement(token);
    if (stmt && (stmt->isValid()))
    {
        node.statement() = stmt;
    }
}

StmtPtr TreeBuilder::visitStatement(const Token &token)
{
    if (token.getValue() == "while")
    {
        StmtPtr stmt(new WhileStmt);
        stmt->visit(*this);
        return stmt;
    }

    if (ASTInterpreter::isActionSupported(token.getValue()))
    {
        std::shared_ptr<ActionStmt> stmt(new ActionStmt);
        stmt->setAction(token.getValue());
        stmt->visit(*this);
        return stmt;
    }

    notify(Error::ErrorInfo("Action not supported: " + token.getValue(), token));
    return StmtPtr(new ActionStmt());
}

/**
 * scans code stream until meets delimiter or EOF
 */
void TreeBuilder::restoreScannerState()
{
    Token token;
    do
    {
        token = m_scanner->scan();
    }
    while ((token.getType() != Token::Type_SEMICOLON)
           && (token.getType() != Token::Type_EOF));
}

} // namespace AST
