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

    _currentTokenStart = streamPos();

    char ch = _stream.peek();
    switch (ch)
    {
    case 0:
        return generateToken(Token::Type_EOF);
    case ';':
        _stream.move();
        return generateToken(Token::Type_SEMICOLON);
    case '(':
        _stream.move();
        return generateToken(Token::Type_LBRACKET);
    case ')':
        _stream.move();
        return generateToken(Token::Type_RBRACKET);
    default:
        if (isalpha(ch))
        {
            std::string value = readIdentifier(_stream);
            Token tk = generateToken(Token::Type_IDENTIFIER);
            tk.setValue(value);
            return tk;
        }
        else
        {
            restoreStream(_stream);
            return generateToken(Token::Type_ILLEGAL);
        }
    }
}

Token Scanner::generateToken(Token::Type type) const
{
    return Token(type, _currentTokenStart, streamPos());
}

TokenPos Scanner::streamPos() const
{
    return TokenPos(_stream.line(), _stream.column());
}
