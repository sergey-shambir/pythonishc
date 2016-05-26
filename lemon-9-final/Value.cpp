#include "Value.h"
#include <stdexcept>
#include <boost/format.hpp>
#include <cmath>

namespace
{

bool IsFarFromZero(double value)
{
    return std::fabs(value) >= std::numeric_limits<double>::epsilon();
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

struct TypeNameVisitor : boost::static_visitor<std::string>
{
    explicit TypeNameVisitor() = default;

    std::string operator ()(double const&) const { return "Number"; }
    std::string operator ()(bool const&) const { return "Boolean"; }
    std::string operator ()(std::string const&) const { return "String"; }
    std::string operator ()(std::exception_ptr const& value) { m_exception = value; return "Error"; }

    std::exception_ptr m_exception;
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
    catch (const std::exception &)
    {
        return CValue::FromError(std::current_exception());
    }
}

CValue CValue::FromDouble(double value)
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
    try
    {
        return CValue::FromDouble(AsDouble());
    }
    catch (std::exception const&)
    {
        return CValue::FromError(std::current_exception());
    }
}

CValue CValue::operator -() const
{
    try
    {
        return CValue::FromDouble(-AsDouble());
    }
    catch (std::exception const&)
    {
        return CValue::FromError(std::current_exception());
    }
}

CValue CValue::operator !() const
{
    try
    {
        return CValue::FromBoolean(!AsBool());
    }
    catch (std::exception const&)
    {
        return CValue::FromError(std::current_exception());
    }
}

CValue CValue::operator <(const CValue &other) const
{
    try
    {
        const auto &type = m_value.type();
        if (type == typeid(std::string))
        {
            return CValue::FromBoolean(AsString() < other.AsString());
        }
        else if (type == typeid(double))
        {
            return CValue::FromBoolean(AsDouble() < other.AsDouble());
        }
    }
    catch (std::exception const&)
    {
    }
    return GenerateError(*this, other, "<");
}

CValue CValue::operator ==(const CValue &other) const
{
    try
    {
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
            bool result = std::fabs(left - right) < std::numeric_limits<double>::epsilon();
            return CValue::FromBoolean(result);
        }
    }
    catch (std::exception const&)
    {
    }
    return GenerateError(*this, other, "==");
}

CValue CValue::operator &&(const CValue &other) const
{
    auto operation = [](bool left, bool right) -> bool {
        return left && right;
    };
    return DoFixedTypeOperation<bool>(*this, other, operation, "&&");
}

CValue CValue::operator ||(const CValue &other) const
{
    auto operation = [](bool left, bool right) -> bool {
        return left || right;
    };
    return DoFixedTypeOperation<bool>(*this, other, operation, "||");
}

CValue CValue::operator +(const CValue &other) const
{
    try
    {
        const auto &type = m_value.type();
        if (type == typeid(std::string))
        {
            return CValue::FromString(AsString() + other.ConvertToString());
        }
        // Transform Value+String to String+Value.
        if (other.m_value.type() == typeid(std::string))
        {
            return CValue::FromString(ConvertToString() + other.AsString());
        }
        else if (type == typeid(double))
        {
            return CValue::FromDouble(AsDouble() + other.ConvertToDouble());
        }
    }
    catch (std::exception const&)
    {
    }
    return GenerateError(*this, other, "+");
}

CValue CValue::operator -(const CValue &other) const
{
    auto operation = [](double left, double right) -> double {
        return left * right;
    };
    return DoFixedTypeOperation<double>(*this, other, operation, "-");
}

CValue CValue::operator *(const CValue &other) const
{
    auto operation = [](double left, double right) -> double {
        return left * right;
    };
    return DoFixedTypeOperation<double>(*this, other, operation, "*");
}

CValue CValue::operator /(const CValue &other) const
{
    auto operation = [](double left, double right) -> double {
        return left / right;
    };
    return DoFixedTypeOperation<double>(*this, other, operation, "/");
}

CValue CValue::operator %(const CValue &other) const
{
    auto operation = [](double left, double right) -> double {
        return std::fmod(left, right);
    };
    return DoFixedTypeOperation<double>(*this, other, operation, "%");
}

template<class TType, class TOperation>
CValue CValue::DoFixedTypeOperation(const CValue &left, const CValue &right, TOperation &&operation, const char *description)
{
    try
    {
        return CValue(Value(operation(boost::get<TType>(left.m_value), boost::get<TType>(right.m_value))));
    }
    catch (std::exception const&)
    {
        return GenerateError(left, right, description);
    }
}

CValue CValue::GenerateError(const CValue &left, const CValue &right, const char *description)
{
    TypeNameVisitor visitor;
    std::string leftType = left.m_value.apply_visitor(visitor);
    std::string rightType = right.m_value.apply_visitor(visitor);

    if (visitor.m_exception)
    {
        return CValue::FromError(visitor.m_exception);
    }

    boost::format formatter("Cannot use '%1%' with %2% and %3%");
    formatter % description % leftType % rightType;

    return CValue::FromErrorMessage(boost::str(formatter));
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
