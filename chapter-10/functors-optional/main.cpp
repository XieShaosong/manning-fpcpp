#include <optional>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include <range/v3/view/transform.hpp>
#include <range/v3/view/filter.hpp>

// We can define a range facade that allows using std::optional
// as if it was a proper range (as in range-v3 library).
// If the optional is empty, the range is empty. If it contains
// a value, it generates a range that contains a single value.

template <typename T1, typename F>
auto transform(const std::optional<T1> &opt, F f)
    -> decltype(f(opt.value()))
{
    if (opt) {
        return f(opt.value());
    } else {
        return {};
    }
}

template <typename T>
class optional_range
    : public ranges::v3::view_facade<optional_range<T>> {

public:
    optional_range() = default;
    explicit optional_range(const std::optional<T> &opt)
        : m_opt_ptr(opt ? &opt.value() : nullptr)
    {
    }

private:
    friend ranges::v3::range_access;
    const T *m_opt_ptr;
    const T & read() const { return *m_opt_ptr; }
    bool equal(ranges::v3::default_sentinel) const {
        return m_opt_ptr == nullptr;
    }
    void next() { m_opt_ptr = nullptr; }
};

template <typename T>
optional_range<T> as_range(const std::optional<T> &opt)
{
    return optional_range<T>(opt);
}

int main(int argc, char *argv[])
{
    std::optional<int> i;

    transform(i, isalnum);

    auto r = as_range(i) | ranges::v3::view::transform(isalnum);

    return 0;
}
