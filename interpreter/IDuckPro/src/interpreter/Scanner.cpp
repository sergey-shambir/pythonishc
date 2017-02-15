#include "Scanner.hpp"

#include <ctype.h>
#include <string.h>

Scanner::Scanner(const std::string &source)
    :_stream(source)
{
}

inline static
void restoreStream(SourceStream& stream)
{
    for (;;)
    {
        stream.move();
        char ch = stream.peek();
        if (isalpha(ch) || (ch == '\0') || (strchr("();", ch)))
            break;
    }
}

inline static
std::string readIdentifier(SourceStream& stream)
{
    std::string ret;
    while (isalpha(stream.peek()))
    {
        ret += tolower(stream.peek());
        stream.move();
    }
    return (ret);
}

Token Scanner::scan()
{
    while (isspace(_stream.peek()))
        _stream.move();

    char ch = _stream.peek();
    switch (ch)
    {
    case 0:
        return Token(Token::Type_EOF);
    case ';':
        _stream.move();
        return Token(Token::Type_SEMICOLON);
    case '(':
        _stream.move();
        return Token(Token::Type_LBRACKET);
    case ')':
        _stream.move();
        return Token(Token::Type_RBRACKET);
    default:
        if (isalpha(ch))
        {
            Token tk(Token::Type_IDENTIFIER);
            tk.setValue(readIdentifier(_stream));
            return tk;
        }
        else
        {
            restoreStream(_stream);
            return Token(Token::Type_ILLEGAL);
        }
    }

    if (ch == '\0') {
        return Token(Token::Type_EOF);
    }
}
