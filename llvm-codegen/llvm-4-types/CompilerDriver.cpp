#include "CompilerDriver.h"
#include "FrontendContext.h"
#include "Parser.h"
#include "Lexer.h"
#include "Grammar.h"
#include "CodegenVisitor.h"
#include "CompilerBackend.h"
#include "TypecheckVisitor.h"
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/IR/Module.h>
#include <sstream>


using std::bind;
using namespace std::placeholders;

static const char C_MAIN_FUNC[] = "main";


class CCompilerDriver::Impl
{
public:
    Impl(std::ostream &errors)
        : m_errors(errors)
        , m_context(errors, m_stringPool)
        , m_codegenContext(m_context)
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

            ThrowIfCompileErrors();
        }
        catch (std::exception const& ex)
        {
            OnFatalError(ex);
            return false;
        }
        return true;
    }

    bool GenerateCodeFromAst()
    {
        try
        {
            std::unique_ptr<CProgramAst> pProgram = m_parser.TakeProgram();
            if (!DetectMainFunction(*pProgram))
            {
                return false;
            }

            CTypecheckVisitor visitor(m_context);
            visitor.RunSemanticPass(*pProgram);

            CCodeGenerator codegen(m_codegenContext);
            unsigned mainId = m_stringPool.Insert(C_MAIN_FUNC);
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

            ThrowIfCompileErrors();

            return true;
        }
        catch (std::exception const& ex)
        {
            OnFatalError(ex);
            return false;
        }
    }

    void ThrowIfCompileErrors()
    {
        if (0 == m_context.GetErrorsCount())
        {
            return;
        }

        const unsigned errorCount = m_context.GetErrorsCount();
        const bool useEnglishPluralForm = (errorCount % 10 != 1);
        std::stringstream message;
        message << errorCount << " compiler " << (useEnglishPluralForm ? "errors" : "error");

        throw std::runtime_error(message.str());
    }

    bool CompileModule(const std::string &outputPath)
    {
        const bool isDebug = false;
        CCompilerBackend backend;
        try
        {
            m_codegenContext.GetModule().dump();
            backend.GenerateObjectFile(m_codegenContext.GetModule(), isDebug, outputPath);
            return true;
        }
        catch (const std::exception &ex)
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
        m_errors << "Fatal error: " << ex.what() << std::endl;
    }

    bool DetectMainFunction(const CProgramAst & ast)
    {
        unsigned mainId = m_stringPool.Insert(C_MAIN_FUNC);
        const FunctionList &functions = ast.GetFunctions();

        bool noMain = std::none_of(functions.begin(), functions.end(), [&](const IFunctionASTUniquePtr &fn) {
            return (fn->GetNameId() == mainId);
        });
        if (noMain)
        {
            m_context.PrintError("`main` function was not defined");
            return false;
        }
        return true;
    }

    std::ostream &m_errors;
    CStringPool m_stringPool;
    CFrontendContext m_context;
    CCodegenContext m_codegenContext;
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

bool CCompilerDriver::Compile(std::istream &input, const std::string &outputPath)
{
    return m_pImpl->ParseAst(input) && m_pImpl->GenerateCodeFromAst() && m_pImpl->CompileModule(outputPath);
}
