#include <optional>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include <range/v3/view/transform.hpp>
#include <range/v3/view/filter.hpp>


// We can also deal with optional values in monadic style. We can chain
// operations that return optional values with monadic binding --
// the composition of transform + join

std::optional<std::string> user_full_name(const std::string& login)
{
    return {};
}

std::optional<std::string> to_html(const std::string& text)
{
    return {};
}



template <typename T1, typename F>
auto transform(const std::optional<T1> &opt, F f)
    -> decltype(std::make_optional(f(opt.value())))
{
    if (opt) {
        return std::make_optional(f(opt.value()));
    } else {
        return {};
    }
}


template <typename T>
std::optional<T> join(const std::optional<std::optional<T>> &opt)
{
    if (opt) {
        return opt.value();
    } else {
        return {};
    }
}

int main(int argc, char *argv[])
{
    std::optional<std::string> login;

    join(transform(
        join(transform(
            login,
            user_full_name)),
        to_html));

    return 0;
}
