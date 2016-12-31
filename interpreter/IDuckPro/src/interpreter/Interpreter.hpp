#ifndef INTERPRETER_HPP_cecaadfd_eaee_4579_b8e2_ece2b3f5e967
#define INTERPRETER_HPP_cecaadfd_eaee_4579_b8e2_ece2b3f5e967

#include <string>
#include <memory>

#include "../duck/IDuck.hpp"
#include "RandomSequence.hpp"

class Scanner;
class Token;

class Interpreter
{
public:
    Interpreter();
    ~Interpreter();

    void setDuck(const std::shared_ptr<IDuck> & duck);

    /**
      Parses one block of code (usually one line).
      Parsing stops after first error.
      */
    void parseBlock(std::string const& block);

    /**
      Dumps available variables list
      */
    void dumpVariablesInfo() const;

    /**
      Returns time in interpreter's world.
      */
    const char* getTime() const;

    /**
      Returns constant pointer on duck's interface
      */
    const IDuck* getDuck() const;

private:
    void handleStatement(Token const& tk, Scanner& scanner);
    void handleWhile(Scanner& scanner);
    void handleCommand(std::string const& command);
    bool getVariable(std::string const& name);
    void emitError(std::string const& description);
    void restoreParser(Scanner& scanner);

    std::shared_ptr<IDuck> _duck;
    RandomSequence _randSeq;
    unsigned _worldTime;
    unsigned _tickNo;
    static const unsigned TICKS_LIMIT = 100;
};

#endif // INTERPRETER_HPP_cecaadfd_eaee_4579_b8e2_ece2b3f5e967
