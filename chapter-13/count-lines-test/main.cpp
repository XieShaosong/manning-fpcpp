// vim: set undofile:
// Program: count_lines_test

#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <forward_list>
#include <iostream>

#include <range/v3/view.hpp>
using namespace ranges::v3;

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>


template < typename Iter
         , typename End>
int count_lines(const Iter& begin, const End& end)
{
    using std::count;
    return count(begin, end, '\n');
}

// Tests

TEST_CASE("Counting newlines in a string", "[counting_lines]")
{
    std::string s = "Hello\nWorld\n";

    REQUIRE(count_lines(begin(s), end(s)) == 2);
}

TEST_CASE("Counting newlines in a string stream", "[counting_lines]")
{
    std::istringstream ss("Hello\nWorld\n");

    REQUIRE(count_lines(std::istream_iterator<char>(ss >> std::noskipws),
                        std::istream_iterator<char>()) == 2);
}

TEST_CASE("Counting newlines in a list", "[counting_lines]")
{
    std::forward_list<char> l {};

    REQUIRE(count_lines(begin(l), end(l)) == 0);
}

TEST_CASE("Counting newlines in a range", "[counting_lines]")
{
    std::string s = "Hello\nWorld\n";
    const auto r = s | view::transform([] (char c) { return toupper(c); });

    REQUIRE(count_lines(begin(r), end(r)) == 2);
}


