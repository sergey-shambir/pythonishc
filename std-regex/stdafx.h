#pragma once

#ifdef _WIN32
#include <SDKDDKVer.h>
#endif

#include <string>
#include <regex>
#include <set>
#include <vector>

#define BOOST_TEST_INCLUDED
#ifdef _WIN32
#pragma warning (disable: 4702)
#pragma warning (disable: 4535)
#endif
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/replace.hpp>
