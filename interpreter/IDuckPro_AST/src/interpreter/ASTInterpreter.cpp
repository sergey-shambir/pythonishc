#include "ASTInterpreter.h"
#include "ast/ast.h"

#include <set>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

enum Variable {
    Variable_DAYLIGHT = 0, Variable_NIGHT, Variable_NOTEAST, Variable_NOTWEST,
    Variable_NOTNORTH, Variable_NOTSOUTH, Variable_PROBABLY, Variable_PERHAPS,
    Variable_IAMLUCKY
};
static const char *const VARIABLES[] = {
    "daylight", "night", "noteast", "notwest",
    "notnorth", "notsouth", "probably", "perhaps",
    "iamlucky"
};

enum Action {
    Action_LEFT = 0, Action_RIGHT, /*Action_UP, Action_DOWN, */Action_QUACK, Action_FLY
};
static const char *const ACTIONS[] = {
    "left", "right", /*"up", "down",*/ "quack", "fly"
};

class ASTInterpreterPrivate : public AST::IVisitor
{
private:
    RandomSequence _randSeq;
    unsigned _worldTime;
    static const unsigned LOOPS_COUNT_LIMIT = 50;
    std::shared_ptr<IDuck> _duck;

    bool getVariable(std::string const& name)
    {
        if (name == VARIABLES[Variable_DAYLIGHT])
        {
            return ((_worldTime - 6) % 24 < 12);
        }
        else if (name == VARIABLES[Variable_NIGHT])
        {
            return ((_worldTime - 6) % 24 >= 12);
        }
        else if (name == VARIABLES[Variable_NOTEAST])
        {
            return _duck->getCourse() != Course_EAST;
        }
        else if (name == VARIABLES[Variable_NOTWEST])
        {
            return _duck->getCourse() != Course_WEST;
        }
        else if (name == VARIABLES[Variable_NOTNORTH])
        {
            return _duck->getCourse() != Course_NORTH;
        }
        else if (name == VARIABLES[Variable_NOTSOUTH])
        {
            return _duck->getCourse() != Course_SOUTH;
        }
        else if (name == VARIABLES[Variable_PROBABLY])
        {
            return (_randSeq() % 13 != 0);
        }
        else if (name == VARIABLES[Variable_PERHAPS])
        {
            return (_randSeq() % 2);
        }
        else if (name == VARIABLES[Variable_IAMLUCKY])
        {
            return (_randSeq() % 77 == 13);
        }
        return false;
    }

public:
    ASTInterpreterPrivate(const std::shared_ptr<IDuck> &duck)
        :_worldTime(6)
        ,_duck(duck)
    {
    }

    virtual ~ASTInterpreterPrivate() { }

    virtual void visitNode(AST::WhileStmt &node)
    {
        unsigned loopNo = 0;
        while (getVariable(node.getVarName()))
        {
            if (loopNo >= LOOPS_COUNT_LIMIT)
                break;
            ++loopNo;

            auto time = _worldTime;
            node.statement()->visit(*this);
            // no action performed
            if (time == _worldTime)
                break;
        }
    }

    virtual void visitNode(AST::ActionStmt &node)
    {
        if (node.getAction() == ACTIONS[Action_FLY])
        {
            _duck->fly();
        }
        else if (node.getAction() == ACTIONS[Action_RIGHT])
        {
            _duck->right();
        }
        else if (node.getAction() == ACTIONS[Action_QUACK])
        {
            _duck->quack();
        }
        else if (node.getAction() == ACTIONS[Action_LEFT])
        {
            _duck->left();
        }
//        else if (node.getAction() == ACTIONS[Action_UP])
//        {
//            //! TODO: implement this case
//        }
//        else if (node.getAction() == ACTIONS[Action_DOWN])
//        {
//            //! TODO: implement this case
//        }
        else
        {
            return;
        }
        ++_worldTime;
    }

    virtual void visitNode(AST::StmtGroup &node)
    {
        const size_t size = node.getSize();
        for (size_t idx = 0; idx < size; ++idx)
        {
            AST::StmtPtr &item = node.getItem(idx);
            item->visit(*this);
        }
    }

    const IDuck *getDuck() const
    {
        return _duck.get();
    }

    unsigned getWorldTime() const
    {
        return _worldTime;
    }
};

ASTInterpreter::ASTInterpreter(const std::shared_ptr<IDuck> &duck)
    :d(new ASTInterpreterPrivate(duck))
{
}

ASTInterpreter::~ASTInterpreter()
{
}

void ASTInterpreter::interpret(const AST::INodePtr &group)
{
    group->visit(*d);
}

const char* ASTInterpreter::getTime() const
{
    const char* const TIMES[] = {
        "12:00 a.m.", "1:00 a.m.",  "2:00 a.m.",
        "3:00 a.m.",  "4:00 a.m.",  "5:00 a.m.",
        "6:00 a.m.",  "7:00 a.m.",  "8:00 a.m.",
        "9:00 a.m.",  "10:00 a.m.", "11:00 a.m.",
        "12:00 p.m.", "1:00 p.m.",  "2:00 p.m.",
        "3:00 p.m.",  "7:00 p.m.",  "7:00 p.m.",
        "6:00 p.m.",  "7:00 p.m.",  "8:00 p.m.",
        "9:00 p.m.",  "10:00 p.m.", "11:00 p.m.",
    };
    const unsigned index = d->getWorldTime() % 24;

    return TIMES[index];
}

const IDuck* ASTInterpreter::getDuck() const
{
    return d->getDuck();
}

void ASTInterpreter::dumpVariablesInfo() const
{
    printf("Available interpreter variables:\n");

    printf("   Time of day: %s, %s\n",
           VARIABLES[Variable_DAYLIGHT],
           VARIABLES[Variable_NIGHT]);

    printf("   Duck's direction: %s, %s, %s, %s\n",
           VARIABLES[Variable_NOTEAST],
           VARIABLES[Variable_NOTWEST],
           VARIABLES[Variable_NOTNORTH],
           VARIABLES[Variable_NOTSOUTH]);

    printf("   Random: %s, %s, %s\n",
           VARIABLES[Variable_PROBABLY],
           VARIABLES[Variable_PERHAPS],
           VARIABLES[Variable_IAMLUCKY]);

    putchar('\n');
}

bool ASTInterpreter::isVariableSupported(const std::string &name)
{
    for (size_t i = 0; i < sizeof(VARIABLES) / sizeof(const char* const); ++i)
    {
        if (VARIABLES[i] == name)
            return true;
    }
    return false;
}

bool ASTInterpreter::isActionSupported(const std::string &name)
{
    for (size_t i = 0; i < sizeof(ACTIONS) / sizeof(const char* const); ++i)
    {
        if (ACTIONS[i] == name)
            return true;
    }
    return false;
}
