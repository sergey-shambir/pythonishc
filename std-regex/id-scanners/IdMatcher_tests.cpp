#include "stdafx.h"
#include "IdMatcher.h"

BOOST_AUTO_TEST_SUITE(CMatchId_class)

BOOST_AUTO_TEST_CASE(does_not_match_empty)
{
	CIdMatcher matcher;
	BOOST_CHECK(false == matcher.IsIdentifier(""));
}

BOOST_AUTO_TEST_CASE(does_not_match_number)
{
	CIdMatcher matcher;
	BOOST_CHECK(false == matcher.IsIdentifier("3187"));
}

BOOST_AUTO_TEST_CASE(does_not_match_list)
{
	CIdMatcher matcher;
	BOOST_CHECK(false == matcher.IsIdentifier("a,b,c"));
}

BOOST_AUTO_TEST_CASE(does_not_match_parenthesis)
{
	CIdMatcher matcher;
	BOOST_CHECK(false == matcher.IsIdentifier("(x)"));
}

BOOST_AUTO_TEST_CASE(does_match_lower_letter)
{
	CIdMatcher matcher;
	BOOST_CHECK(matcher.IsIdentifier("x"));
}

BOOST_AUTO_TEST_CASE(does_match_upper_letter)
{
	CIdMatcher matcher;
	BOOST_CHECK(matcher.IsIdentifier("U"));
}

BOOST_AUTO_TEST_CASE(does_match_lower_word)
{
	CIdMatcher matcher;
	BOOST_CHECK(matcher.IsIdentifier("vector"));
}

BOOST_AUTO_TEST_CASE(does_match_upper_word)
{
	CIdMatcher matcher;
	BOOST_CHECK(matcher.IsIdentifier("fixture"));
}

BOOST_AUTO_TEST_CASE(does_match_word_with_number)
{
	CIdMatcher matcher;
	BOOST_CHECK(matcher.IsIdentifier("var007"));
}

BOOST_AUTO_TEST_CASE(does_match_camel_case)
{
	CIdMatcher matcher;
	BOOST_CHECK(matcher.IsIdentifier("CamelCaseIdentifier"));
}

BOOST_AUTO_TEST_CASE(does_match_underscores_inside)
{
	CIdMatcher matcher;
	BOOST_CHECK(matcher.IsIdentifier("high_order_function"));
}

BOOST_AUTO_TEST_CASE(does_match_underscores_prefix)
{
	CIdMatcher matcher;
	BOOST_CHECK(matcher.IsIdentifier("_hidden"));
}

BOOST_AUTO_TEST_CASE(does_match_underscore)
{
	CIdMatcher matcher;
	BOOST_CHECK(matcher.IsIdentifier("_"));
}

BOOST_AUTO_TEST_SUITE_END()