#pragma once

class CAdvancedIdScanner
{
public:
    CAdvancedIdScanner();

    // Сканирует и сохраняет все идентификаторы в строке,
    // игнорируя комментарии в стиле C: /* comment */.
    void ScanLine(std::string const& text);

    // Возвращает отсортированный список идентификаторов
    std::vector<std::string> GetIds()const;

private:
    using string_iterator = std::string::const_iterator;

    void AdvanceInComment(string_iterator from, string_iterator to);
    void AdvanceNormal(string_iterator from, string_iterator to);
    void AdvanceNoComment(string_iterator from, string_iterator to);

    std::set<std::string> m_ids;
    std::regex m_pattern;
    std::regex m_commentBegin;
    std::regex m_commentEnd;
    bool m_isInComment = false;
};

