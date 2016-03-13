#include <regex>
#include <iostream>
#include <set>

class CIdStore
{
public:
    CIdStore()
        : m_pattern("[a-zA-Z_][a-zA-Z0-9_]*")
    {
    }

    // Stores all identifiers in passed text line.
    void Advance(std::string const& text)
    {
        // We use iterators to avoid scanning the same text twice.
        auto from = text.begin();
        auto to = text.end();
        std::smatch match;
        while (std::regex_search(from, to, match, m_pattern))
        {
            // Store iterator in std::set.
            m_ids.insert(match[0]);
            // Move iterator to the next position after matched ID.
            from += match.prefix().length() + match.length(0);
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

private:
    std::set<std::string> m_ids;
    std::regex m_pattern;
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
