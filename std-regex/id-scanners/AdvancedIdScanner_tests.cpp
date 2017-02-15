#include "stdafx.h"
#include "AdvancedIdScanner.h"
#include <sstream>
#include <boost/optional.hpp>


namespace
{
bool AreIdsEqual(CAdvancedIdScanner const& scanner, std::vector<std::string> const& expectedIds)
{
	std::vector<std::string> ids = scanner.GetIds();
	return (ids == expectedIds);
}

static const char C_MULTILINE_COMMENTED[] = R"***(
do {
    x++;
    counter += 1;
} while (x != y);
/* comments are ignored */
y *= 2;
/*/ we are in comment
    in multiline comment
   */
)***";
}

BOOST_AUTO_TEST_SUITE(CAdvancedIdScanner_class)

BOOST_AUTO_TEST_CASE(scans_empty_string)
{
	CAdvancedIdScanner scanner;
	scanner.ScanLine("");
	BOOST_CHECK(AreIdsEqual(scanner, {}));
}

BOOST_AUTO_TEST_CASE(scans_single_line)
{
	CAdvancedIdScanner scanner;
	scanner.ScanLine("width = rect.right - rect.left;");
	BOOST_CHECK(AreIdsEqual(scanner, { "left", "rect", "right", "width" }));
}

BOOST_AUTO_TEST_CASE(can_ignore_comments)
{
	CAdvancedIdScanner scanner;
	scanner.ScanLine("width = rect.right - rect.left; /* update width */");
	BOOST_CHECK(AreIdsEqual(scanner, { "left", "rect", "right", "width" }));
}

BOOST_AUTO_TEST_CASE(scans_multiple_lines)
{
	CAdvancedIdScanner scanner;
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

BOOST_AUTO_TEST_CASE(can_ignore_multiline_comments)
{
	CAdvancedIdScanner scanner;
	std::stringstream input(C_MULTILINE_COMMENTED);
	std::string line;
	while (std::getline(input, line))
	{
		scanner.ScanLine(line);
	}
	BOOST_CHECK(AreIdsEqual(scanner, { "counter", "do", "while", "x", "y"}));
}

BOOST_AUTO_TEST_SUITE_END()
