#pragma once

#include "DuckProgram.h"
#include <boost/utility/string_ref.hpp>
#include <memory>

/*
 * Возможные улучшения:
 *  - перенаправить вывод ошибок в IErrorsStream
 *   (который мог бы печатать их в произвольном формате)
 *  - заменить string_ref на поток, способный сохранять
 *   текущие номер строки и номер колонки.
 *  - возвращать умный указатель на объект или nullptr
 *    вместо bool в методах, реализующих правила разбора
 */
class Parser
{
public:
    std::unique_ptr<DuckProgram> Parse(const std::string &text);

private:
    bool ParseTranslationUnit();
    bool ParseDuckInfo();
    bool ParseCommandsList();
    bool ParseCommand();
    bool ParseFlyCommand();
    bool ParseQuackCommand();
    bool ParseKeyword(const std::string &word);
    bool ParseIdentifier(std::string &id);
    bool ParseNumber(int64_t &value);
    bool ParseDuckType(std::string &type);
    void TrimSpaces();
    void SkipLine();

    boost::string_ref m_text;
    std::unique_ptr<DuckProgram> m_duck;
    std::exception_ptr m_exception;
};
