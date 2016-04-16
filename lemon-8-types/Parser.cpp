#include "Parser.h"
#include "Token.h"
#include "InterpreterContext.h"
#include <stdlib.h>
#include <new>
#include <iostream>


// pre-declaration of generated functions.
void *ParseGrammarAlloc(void *(*mallocProc)(size_t));
void ParseGrammar(void*, int, SToken, CParser*);
void ParseGrammarFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */);
#ifndef NDEBUG
void ParseGrammarTrace(FILE * TraceFILE, char * zTracePrompt);
#endif


CParser::CParser(CInterpreterContext & context)
    : m_program(context)
{
    auto allocate = [](size_t size) -> void* {
        return new (std::nothrow) char[size];
    };
    m_parser = ParseGrammarAlloc(allocate);
}

CParser::~CParser()
{
    auto retain = [](void *pointer) -> void {
        auto array = reinterpret_cast<char *>(pointer);
        delete[] array;
    };
    ParseGrammarFree(m_parser, retain);
}

bool CParser::Advance(int tokenId, const SToken &tokenData)
{
    ParseGrammar(m_parser, tokenId, tokenData, this);
    return !m_isFatalError;
}

#ifndef NDEBUG
void CParser::StartDebugTrace(FILE *output)
{
    m_tracePrompt = "";
    ParseGrammarTrace(output, &m_tracePrompt[0]);
}
#endif

void CParser::OnError(const SToken &token)
{
    std::cerr << "Syntax error at (" << token.line << "," << token.column << ")." << std::endl;
}

void CParser::OnStackOverflow()
{
    std::cerr << "LALR parser stack overflow occured." << std::endl;
    m_isFatalError = true;
}

void CParser::OnFatalError()
{
    std::cerr << "Fatal error occured." << std::endl;
    m_isFatalError = true;
}

CValue CParser::GetStringLiteral(unsigned stringId) const
{
    std::string str = m_program.GetStringLiteral(stringId);
    return CValue::FromString(str);
}

void CParser::AddStatement(IStatementASTUniquePtr && stmt)
{
    if (stmt)
    {
        m_program.AddStatement(std::move(stmt));
    }
}

void CParser::AddFunction(IFunctionASTUniquePtr &&function)
{
    if (function)
    {
        m_program.AddFunction(std::move(function));
    }
}
