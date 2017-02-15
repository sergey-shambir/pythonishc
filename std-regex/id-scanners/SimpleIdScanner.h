#pragma once

class CSimpleIdScanner
{
public:
	CSimpleIdScanner();

	// Сканирует и сохраняет все идентификаторы в строке
	void ScanLine(std::string const& text);

	// Возвращает отсортированный список идентификаторов
	std::vector<std::string> GetIds()const;

private:
	std::set<std::string> m_ids;
	std::regex m_pattern;
};
