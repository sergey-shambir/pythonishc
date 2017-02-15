#include "Interpreter.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <stdexcept>
#include <vector>

#include "Scanner.hpp"

Interpreter::Interpreter()
    :_duck(0)
    ,_worldTime(6)
{
}

Interpreter::~Interpreter()
{
}

void Interpreter::setDuck(const std::shared_ptr<IDuck> &duck)
{
    _duck = duck;
}

void Interpreter::parseBlock(const std::string &block)
{
    Scanner scanner(block);
    try
    {
        for (Token token = scanner.scan();
             token.getType() != Token::Type_EOF;
             token = scanner.scan())
        {
            if (token.getType() != Token::Type_IDENTIFIER)
                emitError("Unexpected token: " + token.classify());
            else
                handleStatement(token, scanner);
        }
    }
    catch (std::invalid_argument const& error)
    {
        printf("ERROR: %s\n", error.what());
    }
}

const char* Interpreter::getTime() const
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
    const unsigned index = _worldTime % 24;

    return TIMES[index];
}

const IDuck* Interpreter::getDuck() const
{
    return _duck.get();
}

void Interpreter::dumpVariablesInfo() const
{
    puts("Variables:\n"
         "   Time of day: Daylight, Night\n"
         "   Duck's direction: NotEast, NotWest, NotNorth, NotSouth\n"
         "   Random: Probably, Perhaps, AImLucky\n"
         );
}

bool Interpreter::getVariable(const std::string &name)
{
    if (name == "daylight")
    {
        return ((_worldTime - 6) % 24 < 12);
    }
    else if (name == "night")
    {
        return ((_worldTime - 6) % 24 >= 12);
    }
    else if (name == "noteast")
    {
        return _duck->getCourse() != Course_EAST;
    }
    else if (name == "notwest")
    {
        return _duck->getCourse() != Course_WEST;
    }
    else if (name == "notnorth")
    {
        return _duck->getCourse() != Course_NORTH;
    }
    else if (name == "notsouth")
    {
        return _duck->getCourse() != Course_SOUTH;
    }
    else if (name == "probably")
    {
        return (_randSeq() % 13 != 0);
    }
    else if (name == "perhaps")
    {
        return (_randSeq() % 2);
    }
    else if (name == "iamlucky")
    {
        return (_randSeq() % 77 == 13);
    }
    else
    {
        emitError("Variable not found - " + name);
    }
    return false;
}

void Interpreter::handleStatement(const Token &token, Scanner &scanner)
{
    _tickNo = 0;
    if (token.getValue() == "while")
    {
        handleWhile(scanner);
    }
    else
    {
        handleCommand(token.getValue());
        Token after = scanner.scan();
        if (after.getType() != Token::Type_SEMICOLON)
        {
            emitError("Expected semicolon after \"" + token.getValue() + "\"");
        }
    }
}

void Interpreter::handleWhile(Scanner &scanner)
{
    Token token;

    // we store variable for every nested WHILE
    std::vector<std::string> variables;
    for (;;)
    {
        if ((token = scanner.scan()).getType() != Token::Type_LBRACKET)
            emitError("Expected left bracket in WHILE condition");
        else if ((token = scanner.scan()).getType() != Token::Type_IDENTIFIER)
            emitError("Expected variable name in WHILE condition");
        else
        {
            variables.push_back(token.getValue());
            if ((token = scanner.scan()).getType() != Token::Type_RBRACKET)
                emitError("Expected right bracket in WHILE condition");
            else if ((token = scanner.scan()).getType() != Token::Type_IDENTIFIER)
                emitError("Expected statement after WHILE condition");
            else if (token.getValue() == "while")
                continue;
            else
                break;
        }
    }
    {
        Token after = scanner.scan();
        if (after.getType() != Token::Type_SEMICOLON)
        {
            emitError("Expected semicolon after \"" + token.getValue() + "\"");
        }
    }

    // process all WHILE cycles in statement
    for (size_t index = 0; index < variables.size(); ++index)
        if (!getVariable(variables[index]))
        {
            return;
        }
    do
    {
        handleCommand(token.getValue());
    }
    while (getVariable(variables.back()));
}

void Interpreter::handleCommand(const std::string &command)
{
    if (_tickNo > TICKS_LIMIT)
    {
        emitError("Time limit exceeded, probably we have infinite loop");
    }
    if (command == "fly")
    {
        _duck->fly();
    }
    else if (command == "right")
    {
        _duck->right();
    }
    else if (command == "quack")
    {
        _duck->quack();
    }
    else
    {
        emitError("Identifier is not duck\'s command: " + command);
    }
    ++_worldTime;
    ++_tickNo;
}

void Interpreter::emitError(const std::string &description)
{
    throw std::invalid_argument(description);
}
