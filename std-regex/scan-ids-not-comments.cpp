#include <regex>
#include <iostream>
#include <set>

class CIdStore
{
public:
    CIdStore()
        : m_pattern("[a-zA-Z_][a-zA-Z0-9_]*")
        , m_commentBegin("/\\*")
        , m_commentEnd("\\*/")
    {
    }

    // Stores all identifiers in passed text line.
    void Advance(std::string const& text)
    {
        // We use iterators to avoid scanning the same text twice.
        auto from = text.cbegin();
        auto to = text.cend();
        if (m_isInComment)
        {
            AdvanceInComment(from, to);
        }
        else
        {
            AdvanceNormal(from, to);
        }
    }

    // Prints all stored identifiers.
    void PrintAll()
    {
        // std::set removes duplicates and sorts automatically by it's nature.
        for (std::string const& id : m_ids)
        {
            std::cout << id << std::endl;
        }
    }

protected:
    void AdvanceInComment(std::string::const_iterator from,
                          std::string::const_iterator to)
    {
        std::smatch match;
        if (std::regex_search(from, to, match, m_commentEnd))
        {
            m_isInComment = false;
            from += match.prefix().length() + match.length(0);
            AdvanceNormal(from, to);
        }
    }

    void AdvanceNormal(std::string::const_iterator from,
                       std::string::const_iterator to)
    {
        std::smatch match;
        if (std::regex_search(from, to, match, m_commentBegin))
        {
            auto commentStart = from + match.prefix().length();
            AdvanceNoComment(from, commentStart);
            m_isInComment = true;
            AdvanceInComment(commentStart + match.length(0), to);
        }
        else
        {
            AdvanceNoComment(from, to);
        }
    }

    void AdvanceNoComment(std::string::const_iterator from,
                          std::string::const_iterator to)
    {
        std::smatch match;
        while (std::regex_search(from, to, match, m_pattern))
        {
            // Store iterator in std::set.
            m_ids.insert(match[0]);
            // Move iterator to the next position after matched ID.
            from += match.prefix().length() + match.length(0);
        }
    }

private:
    std::set<std::string> m_ids;
    std::regex m_pattern;
    std::regex m_commentBegin;
    std::regex m_commentEnd;
    bool m_isInComment = false;
};

/// Scans input until the end,
/// than prints sorted unique identifiers list.
int main()
{
    CIdStore store;
    std::string line;
    while (std::getline(std::cin, line))
    {
        store.Advance(line);
    }

    store.PrintAll();

    return 0;
}
