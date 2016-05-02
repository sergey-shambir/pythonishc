#include "Value.h"
#include <stdexcept>

namespace
{

bool IsFarFromZero(double value)
{
    return fabs(value) >= std::numeric_limits<double>::epsilon();
}

template <class TFunction>
CValue ExecuteSafely(TFunction && fn)
{
    try
    {
        return fn();
    }
    catch (std::exception const&)
    {
        return CValue::FromError(std::current_exception());
    }
}

std::string ToPrettyString(double value)
{
    const size_t BUFFER_SIZE = 79;
    char buffer[BUFFER_SIZE + 1] = { 0 };
    std::snprintf(buffer, BUFFER_SIZE, "%f", value);

    std::string result = buffer;
    while (!result.empty() && (result.back() == '0'))
    {
        result.pop_back();
    }
    if (!result.empty() && result.back() == '.')
    {
        result.pop_back();
    }

    return result;
}

std::string ToPrettyString(bool value)
{
    return value ? "true" : "false";
}

// Конвертирует значение в Boolean (C++ bool).
struct BooleanConverter : boost::static_visitor<bool>
{
    explicit BooleanConverter() = default;
    bool operator ()(double const& value) const { return IsFarFromZero(value); }
    bool operator ()(bool const& value) const { return value; }
    bool operator ()(std::string const& value) const { return !value.empty(); }
    bool operator ()(std::exception_ptr const&) { return false; }
};

// Конвертирует значение в String (C++ std::string).
struct StringConverter : boost::static_visitor<std::string>
{
    explicit StringConverter() = default;

    std::string operator ()(double const& value) const { return ToPrettyString(value); }
    std::string operator ()(bool const& value) const { return ToPrettyString(value); }
    std::string operator ()(std::string const& value) const { return value; }
    std::string operator ()(std::exception_ptr const& value) { std::rethrow_exception(value); }
};

// Конвертирует значение в Number (C++ double)
struct NumberConverter : boost::static_visitor<double>
{
    explicit NumberConverter() = default;

    double operator ()(double const& value) const { return value; }
    double operator ()(bool const& value) const { return value ? 1. : 0.; }
    double operator ()(std::string const&) const { throw std::runtime_error("cannot convert String to Number"); }
    double operator ()(std::exception_ptr const& value) { std::rethrow_exception(value); }
};

} // anonymous namespace.

CValue::CValue()
{
}

CValue CValue::FromError(const std::exception_ptr &value)
{
    return Value(value);
}

CValue CValue::FromErrorMessage(const std::string &message)
{
    try
    {
        throw std::runtime_error(message);
    }
    catch (std::exception const&)
    {
        return CValue::FromError(std::current_exception());
    }
}

CValue CValue::FromDouble(const double &value)
{
    return Value(value);
}

CValue CValue::FromBoolean(bool value)
{
    return Value(value);
}

CValue CValue::FromString(const std::string &value)
{
    return Value(value);
}

bool CValue::ToBool() const
{
    return ConvertToBool();
}

std::string CValue::ToString() const
{
    return ConvertToString();
}

bool CValue::IsError() const
{
    return (m_value.type() == typeid(std::exception_ptr));
}

bool CValue::AsBool() const
{
    return boost::get<bool>(m_value);
}

const std::string &CValue::AsString() const
{
    return boost::get<std::string>(m_value);
}

double CValue::AsDouble() const
{
    return boost::get<double>(m_value);
}

const std::exception_ptr &CValue::AsError() const
{
    return boost::get<std::exception_ptr>(m_value);
}

CValue CValue::operator +() const
{
    return ExecuteSafely([&] {
        return CValue::FromDouble(AsDouble());
    });
}

CValue CValue::operator -() const
{
    return ExecuteSafely([&] {
        return CValue::FromDouble(-AsDouble());
    });
}

CValue CValue::operator !() const
{
    return ExecuteSafely([&] {
        return CValue::FromBoolean(!AsBool());
    });
}

CValue CValue::operator <(const CValue &other) const
{
    return ExecuteSafely([&] {
        if (m_value.type() == typeid(std::string))
        {
            return CValue::FromBoolean(AsString() < other.AsString());
        }
        else if (m_value.type() == typeid(bool))
        {
            throw std::runtime_error("Cannot use '<' with Boolean values.");
        }
        else if (m_value.type() == typeid(double))
        {
            return CValue::FromBoolean(AsDouble() < other.AsDouble());
        }
        else // exception_ptr.
        {
            return *this;
        }
    });
}

CValue CValue::operator ==(const CValue &other) const
{
    return ExecuteSafely([&] {
        const auto &type = m_value.type();
        if (type == typeid(std::string))
        {
            return CValue::FromBoolean(AsString() == other.AsString());
        }
        else if (type == typeid(bool))
        {
            return CValue::FromBoolean(AsBool() == other.AsBool());
        }
        else if (type == typeid(double))
        {
            double left = AsDouble();
            double right = other.AsDouble();
            bool result = fabs(left - right) < std::numeric_limits<double>::epsilon();
            return CValue::FromBoolean(result);
        }
        else // exception_ptr.
        {
            return *this;
        }
    });
}

CValue CValue::operator &&(const CValue &other) const
{
    return ExecuteSafely([&](){
        const auto &type = m_value.type();
        if (type == typeid(std::string))
        {
            throw std::runtime_error("Cannot use '&&' with String values.");
        }
        else if (type == typeid(bool))
        {
            // Short Curcuit Evaluation работает, т.к. мы используем его и в C++ реализации.
            return CValue::FromBoolean(AsBool() && other.AsBool());
        }
        else if (type == typeid(double))
        {
            throw std::runtime_error("Cannot use '&&' with Double values.");
        }
        else // exception_ptr.
        {
            return *this;
        }
    });
}

CValue CValue::operator ||(const CValue &other) const
{
    return ExecuteSafely([&](){
        const auto &type = m_value.type();
        if (type == typeid(std::string))
        {
            throw std::runtime_error("Cannot use '&&' with String values.");
        }
        else if (type == typeid(bool))
        {
            // Short Curcuit Evaluation работает, т.к. мы используем его и в C++ реализации.
            return CValue::FromBoolean(AsBool() || other.AsBool());
        }
        else if (type == typeid(double))
        {
            throw std::runtime_error("Cannot use '&&' with Double values.");
        }
        else // exception_ptr.
        {
            return *this;
        }
    });
}

CValue CValue::operator +(const CValue &other) const
{
    return ExecuteSafely([&] {
        if (m_value.type() == typeid(std::string))
        {
            return CValue::FromString(AsString() + other.ConvertToString());
        }
        // Transform Value+String to String+Value.
        if (other.m_value.type() == typeid(std::string))
        {
            return CValue::FromString(ConvertToString() + other.AsString());
        }
        else if (m_value.type() == typeid(bool))
        {
            throw std::runtime_error("Cannot use '+' with Boolean values.");
        }
        else if (m_value.type() == typeid(double))
        {
            return CValue::FromDouble(AsDouble() + other.ConvertToDouble());
        }
        else // exception_ptr.
        {
            return *this;
        }
    });
}

CValue CValue::operator -(const CValue &other) const
{
    return ExecuteSafely([&] {
        if (m_value.type() == typeid(bool))
        {
            throw std::runtime_error("Cannot use '-' with Boolean values.");
        }
        else if (m_value.type() == typeid(double))
        {
            return CValue::FromDouble(AsDouble() - other.ConvertToDouble());
        }
        else if (m_value.type() == typeid(std::string))
        {
            throw std::runtime_error("Cannot use '-' with String values.");
        }
        else // exception_ptr.
        {
            return *this;
        }
    });
}

CValue CValue::operator *(const CValue &other) const
{
    return ExecuteSafely([&] {
        if (m_value.type() == typeid(bool))
        {
            throw std::runtime_error("Cannot use '*' with Boolean values.");
        }
        else if (m_value.type() == typeid(double))
        {
            return CValue::FromDouble(AsDouble() * other.ConvertToDouble());
        }
        else if (m_value.type() == typeid(std::string))
        {
            throw std::runtime_error("Cannot use '*' with String values.");
        }
        else // exception_ptr.
        {
            return *this;
        }
    });
}

CValue CValue::operator /(const CValue &other) const
{
    return ExecuteSafely([&] {
        if (m_value.type() == typeid(bool))
        {
            throw std::runtime_error("Cannot use '/' with Boolean values.");
        }
        else if (m_value.type() == typeid(double))
        {
            return CValue::FromDouble(AsDouble() / other.ConvertToDouble());
        }
        else if (m_value.type() == typeid(std::string))
        {
            throw std::runtime_error("Cannot use '/' with String values.");
        }
        else // exception_ptr.
        {
            return *this;
        }
    });
}

CValue CValue::operator %(const CValue &other) const
{
    return ExecuteSafely([&] {
        if (m_value.type() == typeid(bool))
        {
            throw std::runtime_error("Cannot use '%' with Boolean values.");
        }
        else if (m_value.type() == typeid(double))
        {
            return CValue::FromDouble(fmod(AsDouble(), other.ConvertToDouble()));
        }
        else if (m_value.type() == typeid(std::string))
        {
            throw std::runtime_error("Cannot use '%' with String values.");
        }
        else // exception_ptr.
        {
            return *this;
        }
    });
}

CValue::CValue(const CValue::Value &value)
    : m_value(value)
{
}

std::string CValue::ConvertToString() const
{
    StringConverter converter;
    std::string value = m_value.apply_visitor(converter);
    return value;
}

double CValue::ConvertToDouble() const
{
    NumberConverter converter;
    return m_value.apply_visitor(converter);
}

bool CValue::ConvertToBool() const
{
    BooleanConverter converter;
    return m_value.apply_visitor(converter);
}
