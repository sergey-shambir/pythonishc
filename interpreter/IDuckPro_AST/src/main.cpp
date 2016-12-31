#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "duck/MallardDuck.hpp"
#include "duck/ToyDuck.hpp"
#include "duck/RedheadDuck.hpp"

#include "interpreter/ast/AST_TreeBuilder.h"
#include "interpreter/error/IListener.h"
#include "interpreter/ASTInterpreter.h"
#include "interpreter/ast/AST_PrettyPrinter.h"

//! Returns 0 if no duck selected
static std::shared_ptr<IDuck> selectDuck()
{
    puts("Select any duck:\n"
         "M - Mallard duck\n"
         "T - toy duck\n"
         "R - Redhead duck");

    char choice = 0;
    scanf(" %c", &choice);
    while (getchar() != '\n')
        void(0);

    switch (toupper(choice))
    {
    case 'M':
        puts("You drive Mallard duck");
        return std::shared_ptr<IDuck>(new MallardDuck());
    case 'T':
        puts("You drive small toy duck, swimming in bathroom");
        return std::shared_ptr<IDuck>(new ToyDuck());
    case 'R':
        puts("You drive Readhead duck");
        return std::shared_ptr<IDuck>(new RedheadDuck());
    default:
        puts("Nothing selected");
        exit(0);
    }
}

static void printInterpreterState(ASTInterpreter const& interp)
{
    const IDuck* duck = interp.getDuck();
    puts("--------------------------------------------------------");
    printf("Now %s, duck faced %s\n",
           interp.getTime(),
           toString<const char*>(duck->getCourse()));

    Course courseX = Course_EAST;
    int distX = duck->getDistance(courseX);
    if (distX < 0)
    {
        distX = -distX;
        courseX = Course_WEST;
    }
    Course courseY = Course_NORTH;
    int distY = duck->getDistance(courseY);
    if (distY < 0)
    {
        distY = -distY;
        courseY = Course_SOUTH;
    }
    printf("Duck flew %d miles to the %s and %d miles to the %s\n",
           distX, toString<const char*>(courseX),
           distY, toString<const char*>(courseY));
    puts("--------------------------------------------------------");
    putchar('\n');
}

class EchoListener : public Error::IListener
{
    void onError(const Error::ErrorInfo &info) const
    {
        printf("Parsing error: %s\n", info.description.c_str());
        printf("From line %d:%d to line %d:%d\n",
               int(info.start.line), int(info.start.column),
               int(info.end.line), int(info.end.column));
    }
};

int main()
{
    std::shared_ptr<IDuck> duck = selectDuck();
    ASTInterpreter interpreter(duck);
    interpreter.dumpVariablesInfo();
    printInterpreterState(interpreter);

#ifdef _WIN32
    puts("Type code here, press Ctrl+Z to end input:");
#else
    puts("Type code here, press Ctrl+D to end input:");
#endif
    AST::TreeBuilder builder;
    EchoListener listener;
    builder.addListener(&listener);

    int ch;
    std::string line;
    while ((ch = getchar()) != EOF)
    {
        line += ch;
    }

    puts("\nBuilding...");
    Scanner scanner(line);
    AST::INodePtr rootAST = builder.build(&scanner);
    AST::PrettyPrinter printer(std::cout);
    rootAST->visit(printer);

    puts("Interpreting...");
    interpreter.interpret(rootAST);
    printInterpreterState(interpreter);

    return 0;
}
