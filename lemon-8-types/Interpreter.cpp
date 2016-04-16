#include "Interpreter.h"
#include "StringPool.h"
#include "InterpreterContext.h"
#include "Parser.h"
#include "Lexer.h"
#include "Grammar.h"


using std::bind;
using namespace std::placeholders;


class CInterpreter::Impl
{
public:
    Impl()
        : m_context(m_stringPool)
        , m_parser(m_context)
    {
    }

    bool ConsumeLine(std::string const& line)
    {
        ++m_lineNo;
        auto errorHandler = bind(&CInterpreterContext::PrintError, std::ref(m_context), _1);
        CLexer lexer(m_lineNo, line, m_stringPool, errorHandler);
        SToken token;
        for (int tokenId = lexer.Scan(token); tokenId != 0; tokenId = lexer.Scan(token))
        {
            if (!m_parser.Advance(tokenId, token))
            {
                return false;
            }
        }
        token.line = m_lineNo;
        token.column = 1;
        return m_parser.Advance(TK_NEWLINE, token);
    }

private:
    CStringPool m_stringPool;
    CInterpreterContext m_context;
    CParser m_parser;
    unsigned m_lineNo = 0;
};

CInterpreter::CInterpreter()
    : m_pImp(new Impl)
{
}

CInterpreter::~CInterpreter()
{
}

void CInterpreter::EnterLoop(std::istream &input, std::ostream &output, std::ostream &errors)
{
    (void)output;
    (void)errors;

    // Uncomment code below to trace LALR parser shift/reduce.
//    parser.StartDebugTrace(stderr);

    std::string line;
    while (std::getline(input, line))
    {
        if (!m_pImp->ConsumeLine(line))
        {
            return;
        }
    }
}
