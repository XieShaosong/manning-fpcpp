// vim: set undofile:
// Program: 06-generalized-memoization

#include <iostream>
#include <map>
#include <tuple>
#include <unordered_map>

// tag::book[]
template <typename Result, typename... Args>
auto make_memoized(Result (*f)(Args...))
{
    std::map<std::tuple<Args...>, Result> cache;            // <1>

    return [f, cache](Args... args) mutable -> Result
    {
        const auto args_tuple =                             // <2>
            std::make_tuple(args...);                       // <2>
        const auto cached = cache.find(args_tuple);         // <2>

        if (cached == cache.end()) {
            auto result = f(args...);                       // <3>
            cache[args_tuple] = result;                     // <3>
            return result;                                  // <3>

        } else {
            return cached->second;                          // <4>
        }
    };
}
// end::book[]

unsigned int fib(unsigned int n)
{
    std::cout << "Calculating " << n << "!\n";
    return n == 0 ? 0
         : n == 1 ? 1
         : fib(n - 1) + fib(n - 2);
}


int main(int argc, char* argv[])
{
// tag::book_demo[]
    auto fibmemo = make_memoized(fib);

    std::cout << "15! = " << fibmemo(15) << std::endl;      // <1>
    std::cout << "15! = " << fibmemo(15) << std::endl;      // <2>
// end::book_demo[]

    return 0;
}


