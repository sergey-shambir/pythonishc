#include "Parser.h"
#include "Token.h"
#include "FrontendContext.h"
#include <cstdlib>
#include <new>
#include <iostream>
#include <sstream>


// pre-declaration of generated functions.
void *ParseGrammarAlloc(void *(*mallocProc)(size_t));
void ParseGrammar(void*, int, Token, CParser*);
void ParseGrammarFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */);
#ifndef NDEBUG
void ParseGrammarTrace(FILE * TraceFILE, char * zTracePrompt);
#endif


CParser::CParser(CFrontendContext & context)
    : m_context(context)
    , m_pProgram(new CProgramAst)
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

bool CParser::Advance(int tokenId, const Token &tokenData)
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

std::unique_ptr<CProgramAst> CParser::TakeProgram()
{
    return std::move(m_pProgram);
}
#endif

void CParser::OnError(const Token &token)
{
    std::stringstream message;
    message << "Syntax error at (" << token.line << "," << token.column << ")";
    m_context.PrintError(message.str());
}

void CParser::OnStackOverflow()
{
    m_context.PrintError("fatal stack overflow in LALR parser");
    m_isFatalError = true;
}

void CParser::OnFatalError()
{
    m_context.PrintError("fatal error inside LALR parser");
    m_isFatalError = true;
}

std::string CParser::GetStringLiteral(unsigned stringId) const
{
    return m_context.GetString(stringId);
}

void CParser::AddFunction(IFunctionASTUniquePtr &&function)
{
    if (function)
    {
        m_pProgram->AddFunction(std::move(function));
    }
}
