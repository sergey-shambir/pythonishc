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
    std::exception_ptr const& AsError() const;

    // Унарные и бинарные операции с проверкой типов.
    CValue operator +()const;
    CValue operator -()const;
    CValue operator <(const CValue &other)const;
    CValue operator ==(const CValue &other)const;
    CValue operator +(const CValue &other)const;
    CValue operator -(const CValue &other)const;
    CValue operator *(const CValue &other)const;
    CValue operator /(const CValue &other)const;
    CValue operator %(const CValue &other)const;

private:
    CValue(Value const& value);
    std::string ConvertToString()const;
    double ConvertToDouble()const;
    bool ConvertToBool()const;

    Value m_value;
};
