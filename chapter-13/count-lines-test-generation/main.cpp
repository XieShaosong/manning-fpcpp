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

#include <cstdlib>
#include <time.h>

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

#define MAX_NEWLINE_COUNT 50
#define MAX_STRING_LENGTH 10
#define TEST_CASES_PER_LINE_COUNT 10

std::string generate_random_string()
{
    int char_number = rand() % MAX_STRING_LENGTH;
    std::string result;

    for (int i = 0; i < char_number; ++i) {
        char c = rand() % 255;
        if (c == '\n') continue;
        result += c;
    }

    return result;
}

std::string generate_test_case(int line_count)
{
    std::string result;
    for (int i = 0; i < line_count; ++i) {
        result += generate_random_string() + '\n';
    }
    result += generate_random_string();
    return result;
}

auto generate_test_cases(int line_count)
{
    return view::generate(std::bind(generate_test_case, line_count));
}

TEST_CASE("Checling line counts for automatically generated tests", "[counting_lines]")
{
    // rand/srand are good enough for this use-case
    auto seed = time(nullptr);
    std::cout << "Random seed is: " << seed << std::endl;
    srand(seed);

    for (int line_count = 0; line_count < MAX_NEWLINE_COUNT ; ++line_count) {
        for (const auto& collection: generate_test_cases(line_count)
                                     | view::take(TEST_CASES_PER_LINE_COUNT)) {
            REQUIRE(count_lines(begin(collection), end(collection)) == line_count);
        }
    }
}


