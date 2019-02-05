#include "CompilerDriver.h"
#include "StringPool.h"
#include "FrontendContext.h"
#include "Parser.h"
#include "Lexer.h"
#include "Grammar.h"
#include "CodegenVisitor.h"

#include "begin_llvm.h"
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/IR/Module.h>
#include "end_llvm.h"


using std::bind;
using namespace std::placeholders;

static const char C_MAIN_FUNC[] = "main";


class CCompilerDriver::Impl
{
public:
    Impl(std::ostream &errors)
        : m_errors(errors)
        , m_context(errors, m_stringPool)
        , m_parser(m_context)
    {
    }

    bool ParseAst(std::istream & input)
    {
        try
        {
            std::string line;
            while (std::getline(input, line))
            {
                if (!ConsumeLine(line))
                {
                    return false;
                }
            }
        }
        catch (std::exception const& ex)
        {
            OnFatalError(ex);
            return false;
        }
        return true;
    }

    bool CompileAst(std::ostream &output)
    {
        try
        {
            unsigned mainId = m_stringPool.Insert(C_MAIN_FUNC);
            std::unique_ptr<CProgramAst> pProgram = m_parser.TakeProgram();
            const FunctionList &functions = pProgram->GetFunctions();

            bool noMain = std::none_of(functions.begin(), functions.end(), [&](const IFunctionASTUniquePtr &fn) {
                return (fn->GetNameId() == mainId);
            });
            if (noMain)
            {
                m_context.PrintError("`main` function was not defined");
                return false;
            }

            CCodeGenerator codegen(m_context);
            for (const auto &pAst : pProgram->GetFunctions())
            {
                if (pAst->GetNameId() == mainId)
                {
                    codegen.AcceptMainFunction(*pAst);
                }
                else
                {
                    codegen.AcceptFunction(*pAst);
                }
            }

            llvm::raw_os_ostream stream(output);
            m_context.GetModule().print(stream, nullptr);

            return (0 == m_context.GetErrorsCount());
        }
        catch (std::exception const& ex)
        {
            OnFatalError(ex);
            return false;
        }
    }

    void StartDebugTrace()
    {
#ifndef NDEBUG
        m_parser.StartDebugTrace(stderr);
#endif
    }

private:
    bool ConsumeLine(std::string const& line)
    {
        ++m_lineNo;
        auto errorHandler = bind(&CFrontendContext::PrintError, std::ref(m_context), _1);
        CLexer lexer(m_lineNo, line, m_stringPool, errorHandler);
        Token token;
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

    void OnFatalError(std::exception const& ex)
    {
        m_errors << "Fatal exception: " << ex.what() << std::endl;
    }

    std::ostream &m_errors;
    CStringPool m_stringPool;
    CFrontendContext m_context;
    CParser m_parser;
    unsigned m_lineNo = 0;
};

CCompilerDriver::CCompilerDriver(std::ostream &errors)
    : m_pImpl(new Impl(errors))
{
}

CCompilerDriver::~CCompilerDriver()
{
}

void CCompilerDriver::StartDebugTrace()
{
    m_pImpl->StartDebugTrace();
}

bool CCompilerDriver::Compile(std::istream &input, std::ostream &output)
{
    return m_pImpl->ParseAst(input) && m_pImpl->CompileAst(output);
}
