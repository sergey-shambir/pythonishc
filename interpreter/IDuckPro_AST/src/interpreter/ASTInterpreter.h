#pragma once

#include <string>
#include <memory>

#include "RandomSequence.hpp"
#include "ast/AST_StmtGroup.h"
#include "ast/AST_IVisitor.h"

#include "../duck/IDuck.hpp"

class Token;
class ASTInterpreterPrivate;

class ASTInterpreter
{
public:
    ASTInterpreter(const std::shared_ptr<IDuck> & duck);
    ~ASTInterpreter();

    //! Interprets given ast tree (statements group)
    void interpret(const AST::INodePtr &group);

    //! Dumps available variables list
    void dumpVariablesInfo() const;

    //! Returns time in interpreter's world.
    const char* getTime() const;

    //! Returns constant pointer on duck's interface
    const IDuck* getDuck() const;

    static bool isVariableSupported(const std::string &name);
    static bool isActionSupported(const std::string &name);

private:
    std::unique_ptr<ASTInterpreterPrivate> d;
};
