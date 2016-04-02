#include "stdafx.h"
#include "SimpleIdScanner.h"

CSimpleIdScanner::CSimpleIdScanner()
	: m_pattern("[a-zA-Z_][a-zA-Z0-9_]*")
{
}

void CSimpleIdScanner::ScanLine(std::string const& text)
{
	// У функции regex_search есть вариант, принимающий 2 итератора вместо строки,
	// то есть можно использовать итераторы для последовательного движения по строке.
	auto from = text.begin();
	auto to = text.end();
	std::smatch match;
	while (std::regex_search(from, to, match, m_pattern))
	{
		// Сохраняем захваченное регулярным выражением значение.
		m_ids.insert(match[0]);
		// Перемещаем итератор вперёд, на позицию после идентификатора.
		from += match.prefix().length() + match.length(0);
	}
}

std::vector<std::string> CSimpleIdScanner::GetIds()const
{
	std::vector<std::string> result;
	result.reserve(m_ids.size());
	for (std::string const& id : m_ids)
	{
		result.emplace_back(id);
	}

	return result;
}
