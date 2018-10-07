#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include <range/v3/view.hpp>
#include <range/v3/action.hpp>
#include <range/v3/istream_range.hpp>
#include <range/v3/to_container.hpp>

using namespace ranges::v3;

template <typename T>
class err;

template <typename Range>
void write_top_10(const Range& xs)
{
    auto items =
        // Assigning the index to each of the movies.
        // view::ints(1) gives us an infinite range of integers
        // starting with 1. Zipping it with xs will give us
        // a range of pairs. The number of items in this range
        // will be the size of xs.
        view::zip(xs, view::ints(1))
            | view::transform([] (const auto& pair) {
                    return std::to_string(pair.second) +
                           " " + pair.first;
            })
            | view::take(10);

    for (const auto& item: items) {
        std::cout << item << std::endl;
    }
}

int main(int argc, char *argv[])
{
    std::vector<std::string> movies_1 {
            "Meaning of life",
            "Dr Strangelove"
        };

    // If there are less than 10 movies in the vector,
    // all will be printed.  If there are more than 10
    // movies in the vector, only the first 10 will be printed
    write_top_10(movies_1);
}
