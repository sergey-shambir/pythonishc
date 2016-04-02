#pragma once
#include <regex>
#include <cassert>

/// Создавать регулярное выражение при каждом вызове слишком дорого,
/// потому что в конструкторе std::regex происходит компиляция конечного автомата.
/// Этот класс помогает сохранить регулярное выражение между вызовами.
class CIdMatcher
{
public:
	// Конструктор std::regex выбрасывает исключение std::regex_error,
	// если выражение составлено некорректно.
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
