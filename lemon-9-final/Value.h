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

    static CValue FromError(const std::exception_ptr &value);
    static CValue FromErrorMessage(const std::string &message);
    static CValue FromDouble(double value);
    static CValue FromBoolean(bool value);
    static CValue FromString(const std::string &value);

    // Конвертирующее приведение типов, никогда не бросает исключений.
    bool ToBool()const;
    std::string ToString()const;
    bool IsError()const;

    // Прямое приведение типов,
    // Выбрасывает boost::bad_get в случае ошибки.
    bool AsBool()const;
    const std::string & AsString()const;
    double AsDouble()const;
    std::exception_ptr const& AsError() const;

    // Унарные и бинарные операции с проверкой типов.
    CValue operator +()const;
    CValue operator -()const;
    CValue operator !()const;
    CValue operator <(const CValue &other)const;
    CValue operator ==(const CValue &other)const;
    CValue operator &&(const CValue &other)const;
    CValue operator ||(const CValue &other)const;
    CValue operator +(const CValue &other)const;
    CValue operator -(const CValue &other)const;
    CValue operator *(const CValue &other)const;
    CValue operator /(const CValue &other)const;
    CValue operator %(const CValue &other)const;

private:
    template<class TType, class TOperation>
    static CValue DoFixedTypeOperation(const CValue &left, const CValue &right, TOperation && op, const char *description);

    static CValue GenerateError(const CValue &left, const CValue &right, const char *description);

    CValue(Value const& value);
    std::string ConvertToString()const;
    double ConvertToDouble()const;
    bool ConvertToBool()const;

    Value m_value;
};
