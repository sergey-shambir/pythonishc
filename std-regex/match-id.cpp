#include <regex>
#include <iostream>


/// We use 'std::regex_match' function.
/// @see http://www.cplusplus.com/reference/regex/regex_match/
bool MatchIdentifier(std::string const& text)
{
    try
    {
        // May throw std::regex_error if expression is incorrect.
        std::regex pattern("[a-zA-Z_][a-zA-Z0-9_]*");
        return std::regex_match(text, pattern);
    }
    catch (std::regex_error const& ex)
    {
        std::cerr << ex.what() << " #" << ex.code() << std::endl;
        return false;
    }
}

/// Creating 'std::regex' each time is too expensive
/// because it compiles DFA in constructor.
/// This class helps to avoid multiple creation.
class CIdMatcher
{
public:
    CIdMatcher()
        : m_pattern("[a-zA-Z_][a-zA-Z0-9_]*")
    {
    }

    bool IsIdentifier(std::string const& text)const
    {
        return std::regex_match(text, m_pattern);
    }

private:
    std::regex m_pattern;
};

int main()
{
    CIdMatcher matcher;
    std::string line;
    while (std::getline(std::cin, line))
    {
        bool isId = matcher.IsIdentifier(line);
        std::cout << (isId ? "is identifier" : "is not identifier") << std::endl;
    }

    return 0;
}
