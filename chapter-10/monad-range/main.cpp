#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <functional>

#include <filesystem>

#include <range/v3/view/transform.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/join.hpp>

using namespace ranges::v3;


namespace fs = std::filesystem;

// Monadic binding is a composition of transform and join.
// We already have those defined in the ranges library,
// so we just need to apply them to the input range.
template <typename Range, typename F>
auto mbind(const Range& range, F&& f)
{
    return range | view::transform(std::forward<F>(f))
                 | view::join;
}

// Ranges views can be composed using the same pipe operator
// used for applying a view to a range.
template <typename F>
decltype(auto) mbind(F&& f)
{
    return view::transform(std::forward<F>(f))
         | view::join;
}


auto files_in_dir(const fs::directory_entry &dir)
{
    return make_iterator_range(fs::directory_iterator{dir.path()},
                               fs::directory_iterator{});
};


int main(int argc, char *argv[])
{
    auto directories =
        ranges::make_iterator_range(fs::directory_iterator{".."},
                                    fs::directory_iterator{})
            | view::filter([] (auto&& item) { return item.is_directory(); });

    std::cout << "Listing files with xs | mbind(f)" << std::endl;
    for (const auto& file: directories | mbind(files_in_dir)) {
        std::cout << file << std::endl;
    }

    return 0;
}
