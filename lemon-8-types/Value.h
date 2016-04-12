#pragma once
#include <boost/variant.hpp>
#include <iostream>

class CValue
{
public:
    using Value = boost::variant<
        std::exception_ptr,
        bool,
        double,
        std::string
    >;

    CValue();

    static CValue FromError(std::exception_ptr const& value);
    static CValue FromErrorMessage(std::string const& message);
    static CValue FromDouble(double const& value);
    static CValue FromBoolean(bool value);
    static CValue FromString(std::string const& value);

    // Конвертирующее приведение типов, никогда не бросает исключений.
    bool ToBool()const;
    std::string ToString()const;
    bool IsError()const;

    // Прямое приведение типов,
    // Выбрасывает boost::bad_cast в случае ошибки.
    bool AsBool()const;
    const std::string & AsString()const;
    double AsDouble()const;

    // Унарные и бинарные операции с проверкой типов.
    CValue operator +()const;
    CValue operator -()const;
    CValue operator <(const CValue &other)const;
    CValue operator +(const CValue &other)const;
    CValue operator -(const CValue &other)const;
    CValue operator *(const CValue &other)const;
    CValue operator /(const CValue &other)const;
    CValue operator %(const CValue &other)const;

    // TODO: bool LessThan().

    // TODO: bool FuzzyEquals().

private:
    CValue(Value const& value);
    std::string ConvertToString(bool printError)const;
    double ConvertToDouble()const;
    bool ConvertToBool()const;

    Value m_value;
};

std::ostream & operator <<(std::ostream & out, CValue const& value);
