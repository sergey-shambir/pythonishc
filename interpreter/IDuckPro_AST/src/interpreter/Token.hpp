#pragma once

#include <string>

struct TokenPos
{
    size_t line;
    size_t column;

    TokenPos()
        :line(0), column(0)
    {
    }

    TokenPos(size_t vLine, size_t vColumn)
        :line(vLine), column(vColumn)
    {
    }
};

class Token
{
public:
    enum Type {
        Type_EOF,
        Type_LBRACKET,
        Type_RBRACKET,
        Type_SEMICOLON,
        Type_IDENTIFIER,
        Type_ILLEGAL
    };

    Token()
        :_type(Type_EOF)
    {
    }

    explicit Token(Type type,
                   const TokenPos &start,
                   const TokenPos &end)
        :_type(type)
        ,_start(start)
        ,_end(end)
    {
    }

    explicit Token(Type type,
                   const size_t &sline, const size_t &scolumn,
                   const size_t &eline, const size_t &ecolumn)
        :_type(type)
        ,_start(sline, scolumn)
        ,_end(eline, ecolumn)
    {
    }

    std::string classify() const
    {
        switch (_type)
        {
        case Type_EOF:
            return "end of line";
        case Type_LBRACKET:
            return "left bracket";
        case Type_RBRACKET:
            return "right bracket";
        case Type_SEMICOLON:
            return "semicolon";
        case Type_IDENTIFIER:
            return "identifier";
        default:
            return "illegal";
        }
    }

    Type getType() const
    {
        return _type;
    }

    void setValue(std::string const& value)
    {
        _value = value;
    }

    const std::string &getValue() const
    {
        return _value;
    }

    const TokenPos &getStartPos() const
    {
        return _start;
    }

    const TokenPos &getEndPos() const
    {
        return _end;
    }

private:
    Type _type;
    std::string _value;
    TokenPos _start;
    TokenPos _end;
};
