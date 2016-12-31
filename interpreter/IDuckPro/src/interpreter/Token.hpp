#ifndef TOKEN_HPP_99d89768_e672_47d2_8072_8d676464fbf3
#define TOKEN_HPP_99d89768_e672_47d2_8072_8d676464fbf3

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
    explicit Token(Type vType = Type_EOF)
        :_type(vType)
    {}

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

    std::string const& getValue() const
    {
        return _value;
    }

private:
    Type _type;
    std::string _value;
};

#endif // TOKEN_HPP_99d89768_e672_47d2_8072_8d676464fbf3
