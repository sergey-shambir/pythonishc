#include "BatchParser.h"
#include "Token.h"
#include "InterpreterContext.h"
#include <stdlib.h>
#include <new>
#include <iostream>


// pre-declaration of generated functions.
void *ParseBatchGrammarAlloc(void *(*mallocProc)(size_t));
void ParseBatchGrammar(void*, int, SToken, CBatchParser*);
void ParseBatchGrammarFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */);
#ifndef NDEBUG
void ParseBatchGrammarTrace(FILE * TraceFILE, char * zTracePrompt);
#endif


CBatchParser::CBatchParser(CInterpreterContext & context)
    : m_context(context)
{
    auto allocate = [](size_t size) -> void* {
        return new (std::nothrow) char[size];
    };
    m_parser = ParseBatchGrammarAlloc(allocate);
}

CBatchParser::~CBatchParser()
{
    auto retain = [](void *pointer) -> void {
        auto array = reinterpret_cast<char *>(pointer);
        delete[] array;
    };
    ParseBatchGrammarFree(m_parser, retain);
}

bool CBatchParser::Advance(int tokenId, const SToken &tokenData)
{
    ParseBatchGrammar(m_parser, tokenId, tokenData, this);
    return !m_isErrorState;
}

#ifndef NDEBUG
void CBatchParser::StartDebugTrace(FILE *output)
{
    m_tracePrompt = "";
    ParseBatchGrammarTrace(output, &m_tracePrompt[0]);
}
#endif

void CBatchParser::OnError(const SToken &token)
{
    std::cerr << "Syntax error at (" << token.line << "," << token.column << ")." << std::endl;
    m_isErrorState = true;
}

void CBatchParser::OnStackOverflow()
{
    std::cerr << "LALR parser stack overflow occured." << std::endl;
    m_isErrorState = true;
}

void CBatchParser::AssignVariable(unsigned stringId, double value)
{
    m_context.AssignVariable(stringId, value);
}

double CBatchParser::GetVariableValue(unsigned stringId)
{
    return m_context.GetVariableValue(stringId);
}

void CBatchParser::PrintResult(double value)
{
    m_context.PrintResult(value);
}
