// vim: set undofile:
// Program: 06-recursive-memoization

#include <iostream>
#include <map>
#include <mutex>
#include <tuple>
#include <type_traits>
#include <unordered_map>

namespace detail {

// tag::book_helper[]
template <class Sig, class F>
class memoize_helper;

template <class Result, class... Args, class F>
class memoize_helper<Result(Args...), F> {
private:
    using function_type = F;
    using args_tuple_type
        = std::tuple<std::decay_t<Args>...>;

    function_type f;
    mutable std::map<args_tuple_type, Result> m_cache;       // <1>
    mutable std::recursive_mutex m_cache_mutex;              // <1>

public:
    template <typename Function>
    memoize_helper(int, Function&& f)                        // <2>
        : f(f)                                               // <2>
    {
    }

    memoize_helper(const memoize_helper& other)              // <2>
        : f(other.f)                                         // <2>
    {
    }

    template <class... InnerArgs>
    Result operator()(InnerArgs&&... args) const
    {
        std::unique_lock<std::recursive_mutex>
                lock{m_cache_mutex};

        const auto args_tuple =                              // <3>
            std::make_tuple(args...);                        // <3>
        const auto cached = m_cache.find(args_tuple);        // <3>

        if (cached != m_cache.end()) {
            return cached->second;                           // <4>

        } else {
            auto&& result = f(                               // <5>
                    *this,                                   // <5>
                    std::forward<InnerArgs>(args)...);       // <5>
            m_cache[args_tuple] = result;                    // <5>
            return result;                                   // <5>
        }
    }
};
// end::book_helper[]

} // namespace detail

using detail::memoize_helper;

// tag::book_maker[]
template <class Sig, class F>
memoize_helper<Sig, std::decay_t<F>>
make_memoized_r(F&& f)
{
    return {0, std::forward<F>(f)};
}
// end::book_maker[]


int main(int argc, char* argv[])
{
// tag::book_demo[]
    auto fibmemo = make_memoized_r<
                unsigned int(unsigned int)>(
        [](auto& fib, unsigned int n) {
            std::cout << "Calculating " << n << "!\n";
            return n == 0 ? 0
                 : n == 1 ? 1
                 : fib(n - 1) + fib(n - 2);
        });
// end::book_demo[]

    std::cout << "15! = " << fibmemo(15) << std::endl;       // <1>
    std::cout << "15! = " << fibmemo(15) << std::endl;       // <2>

    return 0;
}
