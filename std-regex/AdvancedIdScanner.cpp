#include "stdafx.h"
#include "AdvancedIdScanner.h"

CAdvancedIdScanner::CAdvancedIdScanner()
	: m_pattern("[a-zA-Z_][a-zA-Z0-9_]*")
	, m_commentBegin("/\\*")
	, m_commentEnd("\\*/")
{
}

void CAdvancedIdScanner::ScanLine(std::string const& text)
{
	// ” функции regex_search есть вариант, принимающий 2 итератора вместо строки,
	// то есть можно использовать итераторы дл€ последовательного движени€ по строке.
	auto from = text.cbegin();
	auto to = text.cend();
	if (m_isInComment)
	{
		AdvanceInComment(from, to);
	}
	else
	{
		AdvanceNormal(from, to);
	}
}

std::vector<std::string> CAdvancedIdScanner::GetIds()const
{
	std::vector<std::string> result;
	result.reserve(m_ids.size());
	for (std::string const& id : m_ids)
	{
		result.emplace_back(id);
	}

	return result;
}

void CAdvancedIdScanner::AdvanceInComment(string_iterator from, string_iterator to)
{
	std::smatch match;
	if (std::regex_search(from, to, match, m_commentEnd))
	{
		m_isInComment = false;
		from += match.prefix().length() + match.length(0);
		AdvanceNormal(from, to);
	}
}

void CAdvancedIdScanner::AdvanceNormal(string_iterator from, string_iterator to)
{
	std::smatch match;
	if (std::regex_search(from, to, match, m_commentBegin))
	{
		auto commentStart = from + match.prefix().length();
		AdvanceNoComment(from, commentStart);
		m_isInComment = true;
		AdvanceInComment(commentStart + match.length(0), to);
	}
	else
	{
		AdvanceNoComment(from, to);
	}
}

void CAdvancedIdScanner::AdvanceNoComment(string_iterator from, string_iterator to)
{
	std::smatch match;
	while (std::regex_search(from, to, match, m_pattern))
	{
		// Store iterator in std::set.
		m_ids.insert(match[0]);
		// Move iterator to the next position after matched ID.
		from += match.prefix().length() + match.length(0);
	}
}
