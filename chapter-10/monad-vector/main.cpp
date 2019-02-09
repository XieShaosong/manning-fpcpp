#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <functional>

template <typename T>
std::vector<T> make_vector(T&& value)
{
    return { std::forward<T>(value) };
}


template <typename T, typename F>
auto mbind(const std::vector<T>& xs, F f)
{
    // We will return a new vector containing transformed items.
    // The function f takes a value of type T and returns
    // a vector of some type R -- std::vector<R>.
    // The mbind function calls f on each of the values in xs
    // and joins all the resulting vectors into one.
    // This means that mbind also returns std::vector<R>
    using result_vector_type =
        std::invoke_result_t<F, T>;

    // invoke_result_t can be replaced with decltype(f(xs[0]))
    // if you don't have a C++17-compliant compiler

    result_vector_type result;

    for (const auto& x: xs) {
        auto transformed = std::invoke(f, x);

        // Moving all items from transformed into the result
        result.insert(
                result.end(),
                std::make_move_iterator(transformed.begin()),
                std::make_move_iterator(transformed.end()));
    }

    return result;
}


// If we want to support the xs | mbind(f) syntax,
// we also need a unary mbind function which takes just
// the transformation used for monadic binding.
// This function will just create a dummy helper object
// for which we can define operator| that will
// perform the actual monadic binding

namespace detail {
    template <typename F>
    struct mbind_helper {
        F f;
    };

} // namespace detail


template <typename F>
detail::mbind_helper<F> mbind(F&& f)
{
    return {std::forward<F>(f)};
}

template <typename T, typename F>
auto operator| (const std::vector<T>& xs, const detail::mbind_helper<F>& helper)
{
    return mbind(xs, helper.f);
}




int main(int argc, char *argv[])
{
    std::vector<std::string> directories{ "src", "build" };

    // Just for testing purposes, let's create a dummy function
    // which pretends to list files in a directory
    auto files_in_dir = [] (const std::string &dir) {
        return std::vector<std::string> {
            dir + "/file1.txt",
            dir + "/file2.txt"
        };
    };

    std::cout << "Listing files with mbind(xs, f)" << std::endl;
    for (const auto& file: mbind(directories, files_in_dir)) {
        std::cout << file << std::endl;
    }

    std::cout << "Listing files with xs | mbind(f)" << std::endl;
    for (const auto& file: directories | mbind(files_in_dir)) {
        std::cout << file << std::endl;
    }

    return 0;
}
