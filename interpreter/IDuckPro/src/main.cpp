#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "interpreter/Interpreter.hpp"
#include "duck/MallardDuck.hpp"
#include "duck/ToyDuck.hpp"
#include "duck/RedheadDuck.hpp"

//! Returns 0 if no duck selected
static
void selectDuck(Interpreter& interp)
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
        interp.setDuck(std::shared_ptr<IDuck>(new MallardDuck()));
        break;
    case 'T':
        puts("You drive small toy duck, swimming in bathroom");
        interp.setDuck(std::shared_ptr<IDuck>(new ToyDuck()));
        break;
    case 'R':
        puts("You drive Readhead duck");
        interp.setDuck(std::shared_ptr<IDuck>(new RedheadDuck()));
        break;
    default:
        puts("Nothing selected");
        exit(0);
    }
}

static
void printInterpreterState(Interpreter const& interp)
{
    const IDuck* duck = interp.getDuck();
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
    putchar('\n');
}

int main()
{
    Interpreter interp;
    selectDuck(interp);

    interp.dumpVariablesInfo();
    printInterpreterState(interp);

#ifdef _WIN32
    puts("Type code here, press Ctrl+Z to end input:");
#else
    puts("Type code here, press Ctrl+D to end input:");
#endif

    int ch;
    std::string line;
    while ((ch = getchar()) != EOF)
    {
        if (ch == '\n')
        {
            interp.parseBlock(line);
            line.clear();
        }
        else
            line += ch;
    }
    interp.parseBlock(line);

    printInterpreterState(interp);
}
