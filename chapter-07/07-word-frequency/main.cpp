#include <iostream>
#include <vector>
#include <functional>

#include <range/v3/view.hpp>
#include <range/v3/action.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/to_container.hpp>

using namespace ranges::v3;
using namespace std::placeholders;

std::string string_to_lower(const std::string &s) {
    return s | view::transform(tolower);
}

std::string string_only_alnum(const std::string &s) {
    return s | view::filter(isalnum);
}

int main(int argc, char *argv[])
{
    const int n = argc <= 1
                    ? 10
                    : atoi(argv[1]);

    const auto words =
               // Getting a range of words (tokens) from cin
               istream_range<std::string>(std::cin)

               // Converting all words to lower-case
               | view::transform(string_to_lower)

               // Removing non alphanumeric characters from the words
               | view::transform(string_only_alnum)

               // Some words could have only contained non alphanumeric characters
               | view::remove_if(&std::string::empty)

               // For sorting, we need a random-access collection
               | to_vector | action::sort;


    const auto results =
               words

               // Grouping the same words
               | view::group_by(std::equal_to<>())

               // Creating a pair that consists of a word and its
               // frequency
               | view::transform([] (const auto &group) {
                         const auto begin       = std::begin(group);
                         const auto end         = std::end(group);
                         const int  count       = distance(begin, end);
                         const std::string word = *begin;

                         return std::make_pair(count, word);
                     })

               // Sorting the resulting range by word frequencies
               | to_vector | action::sort;


    for (auto value: results | view::reverse // Most frequent words first
                             | view::take(n) // Taking the top `n` results
            ) {
        std::cout << value.first << " " << value.second << std::endl;
    }

    return 0;
}
