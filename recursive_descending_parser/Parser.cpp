#include "Parser.h"
#include <boost/algorithm/string.hpp>

std::unique_ptr<DuckProgram> Parser::Parse(const std::string &text)
{
    // Сбрасываем флаг ошибки, создаём объект утки
    m_exception = nullptr;
    m_duck = std::make_unique<DuckProgram>();
    // Формируем срез строки.
    m_text = boost::string_ref(text);
    // Выполняем разбор по основному правилу: translation_unit
    ParseTranslationUnit();
    // Если на уровнях ниже произошла ошибка,
    //  будет сохранён объект исключения для выброса.
    if (m_exception)
    {
        std::rethrow_exception(m_exception);
    }

    // Выбрасываем наружу владеющий указатель на утку.
    return std::move(m_duck);
}

// Правило translation_unit = duck_info command_list
bool Parser::ParseTranslationUnit()
{
    if (!ParseDuckInfo())
    {
        throw std::runtime_error("expected duck info");
    }
    if (!ParseCommandsList())
    {
        throw std::runtime_error("expected commands list");
    }
    return false;
}

// Правило duck_info = ‘for’ ‘duck’ IDENTIFIER ‘as’ duck_type
bool Parser::ParseDuckInfo()
{
    if (!ParseKeyword("for"))
    {
        return false;
    }
    if (!ParseKeyword("duck"))
    {
        throw std::runtime_error("expected 'duck' keyword");
    }
    std::string name;
    if (!ParseIdentifier(name))
    {
        throw std::runtime_error("expected identifier (duck name)");
    }
    if (!ParseKeyword("as"))
    {
        throw std::runtime_error("expected 'as' keyword");
    }
    std::string type;
    if (!ParseDuckType(type))
    {
        throw std::runtime_error("expected duck type ('mallard', 'toy' or 'robot')");
    }

    m_duck->SetName(name);
    m_duck->SetType(type);

    return true;
}

// Правило command_list = command | command_list command
// Если при разборе command возникла ошибка,
//  пропускает целую строку и продолжает разбор, отметив наличие ошибки.
bool Parser::ParseCommandsList()
{
    bool parsed = false;
    while (true)
    {
        try
        {
            if (!ParseCommand())
            {
                break;
            }
            parsed = true;
        }
        catch (const std::exception &ex)
        {
            std::cerr << "Error (command ignored): " << ex.what() << std::endl;
            SkipLine();
            // Если была хотя бы одна ошибка, программа целиком некорректна,
            //  но сообщить об этом надо позже.
            if (!m_exception)
            {
                m_exception = std::make_exception_ptr(
                    std::runtime_error("Fatal error: one or more errors in file")
                );
            }
        }
    }

    return parsed;
}

// Правило command = fly_command | quack_command | say_command
// TODO: вариант say_command не обрабатывается
//       say_command = ‘say’ STRING_LITERAL
bool Parser::ParseCommand()
{
    TrimSpaces();
    if (m_text.empty())
    {
        return false;
    }

    if (ParseFlyCommand())
    {
        return true;
    }
    if (ParseQuackCommand())
    {
        return true;
    }

    throw std::logic_error("unknown command syntax");
}

// Правило fly_command = ‘fly’ NUMBER ‘seconds’ | ‘fly’ NUMBER ‘meters’
bool Parser::ParseFlyCommand()
{
    if (!ParseKeyword("fly"))
    {
        return false;
    }
    int64_t flyParam = 0;
    if (!ParseNumber(flyParam))
    {
        throw std::runtime_error("expected number");
    }
    if (ParseKeyword("seconds"))
    {
        m_duck->AddTimedFlyCommand(std::chrono::seconds(flyParam));
        return true;
    }
    if (ParseKeyword("meters"))
    {
        m_duck->AddFlyCommandWithMeters(flyParam);
        return true;
    }
    throw std::runtime_error("unknown 'fly' command syntax");
}

// Правило quack_command = ‘quack’
bool Parser::ParseQuackCommand()
{
    if (ParseKeyword("quack"))
    {
        m_duck->AddQuackCommand();
        return true;
    }
    return false;
}

// Ищет ключевое слово во входном потоке.
// Если слово есть, отсекает часть строки и возвращает true.
bool Parser::ParseKeyword(const std::string &word)
{
    TrimSpaces();
    if (boost::starts_with(m_text, word))
    {
        m_text.remove_prefix(word.size());
        return true;
    }
    return false;
}

// Лексическое правило IDENTIFIER = [A-Za-z]+
bool Parser::ParseIdentifier(std::string &id)
{
    TrimSpaces();
    std::string idValue;
    while (!m_text.empty() && ::isalpha(m_text.front()))
    {
        idValue += m_text.front();
        m_text.remove_prefix(1);
    }
    id = idValue;
    return !id.empty();
}

// Лексическое правило NUMBER = [0..9]+
bool Parser::ParseNumber(int64_t &value)
{
    TrimSpaces();
    char *endptr = nullptr;
    int64_t temp = ::strtoll(m_text.data(), &endptr, 10);
    if (*endptr == '\0' || ::isspace(*endptr))
    {
        const size_t numLength = endptr - m_text.data();
        m_text.remove_prefix(numLength);
        value = temp;
        return true;
    }
    return false;
}

// Правило duck_type = ‘mallard’ | ‘toy’ | ‘robot’
bool Parser::ParseDuckType(std::string &type)
{
    // ! Дурной код !
    // Сохранять тип утки как строку - дурное решение,
    //  при расширении числа типов или добавлении новых
    //  операций над ними легко допустить ошибку.
    const std::vector<std::string> knownTypes = {
        "toy", "mallard", "robot"
    };
    for (const auto &knownType : knownTypes)
    {
        if (ParseKeyword(knownType))
        {
            type = knownType;
            return true;
        }
    }
    return false;
}

// Убирает незначимые пробельные символы из входного потока.
void Parser::TrimSpaces()
{
    while (!m_text.empty() && ::isspace(m_text.at(0)))
    {
        m_text.remove_prefix(1);
    }
}

// Пропускает одну строку исходного кода целиком.
void Parser::SkipLine()
{
    while (!m_text.empty() && (m_text.front() != '\n'))
    {
        m_text.remove_prefix(1);
    }
}
