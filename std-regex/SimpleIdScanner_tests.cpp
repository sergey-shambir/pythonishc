#include "stdafx.h"
#include "SimpleIdScanner.h"

namespace
{
bool AreIdsEqual(CSimpleIdScanner const& scanner, std::vector<std::string> const& expectedIds)
{
	std::vector<std::string> ids = scanner.GetIds();
	return (ids == expectedIds);
}
}

BOOST_AUTO_TEST_SUITE(CSimpleIdScanner_class)

BOOST_AUTO_TEST_CASE(scans_empty_string)
{
	CSimpleIdScanner scanner;
	scanner.ScanLine("");
	BOOST_CHECK(AreIdsEqual(scanner, {}));
}

BOOST_AUTO_TEST_CASE(scans_single_line)
{
	CSimpleIdScanner scanner;
	scanner.ScanLine("width = rect.right - rect.left;");
	BOOST_CHECK(AreIdsEqual(scanner, {"left", "rect", "right", "width"}));
}

BOOST_AUTO_TEST_CASE(cannot_ignore_comments)
{
	CSimpleIdScanner scanner;
	scanner.ScanLine("width = rect.right - rect.left; /* update width */");
	BOOST_CHECK(AreIdsEqual(scanner, { "left", "rect", "right", "update", "width" }));
}

BOOST_AUTO_TEST_CASE(scans_multiple_lines)
{
	CSimpleIdScanner scanner;
	std::vector<std::string> lines = {
		"width = rect.right - rect.left;",
		"height = rect.bottom - rect.top",
		"aspect = width / height;"
	};
	for (auto const& line : lines)
	{
		scanner.ScanLine(line);
	}
	BOOST_CHECK(AreIdsEqual(scanner, { "aspect", "bottom", "height", "left", "rect", "right", "top", "width" }));
}

BOOST_AUTO_TEST_SUITE_END()
