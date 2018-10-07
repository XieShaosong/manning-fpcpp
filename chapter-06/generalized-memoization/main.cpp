// vim: set undofile:
// Program: 06-generalized-memoization

#include <iostream>
#include <map>
#include <tuple>
#include <unordered_map>

// see section 6.3 and listing 6.7
template <typename Result, typename... Args>
auto make_memoized(Result (*f)(Args...))
{
    // Creates a cache that maps tuples of arguments to the calculated
    // results. If you wanted to use this in a multithreaded environment,
    // you’d need to synchronize the changes to it with a mutex,
    // as in listing 6.1.
    std::map<std::tuple<Args...>, Result> cache;

    return [f, cache](Args... args) mutable -> Result
    {
        // Lambda that gets the arguments and checks whether the result is
        // already cached
        const auto args_tuple =
            std::make_tuple(args...);
        const auto cached = cache.find(args_tuple);

        if (cached == cache.end()) {
            // In case of a cache miss, calls  the function and stores the
            // result to the cache
            auto result = f(args...);
            cache[args_tuple] = result;
            return result;

        } else {
            // If the result is found in the cache, returns it to the caller
            return cached->second;
        }
    };
}


unsigned int fib(unsigned int n)
{
    std::cout << "Calculating " << n << "!\n";
    return n == 0 ? 0
         : n == 1 ? 1
         : fib(n - 1) + fib(n - 2);
}


int main(int argc, char* argv[])
{
    auto fibmemo = make_memoized(fib);

    std::cout << "15! = " << fibmemo(15) << std::endl;      // <1>
    std::cout << "15! = " << fibmemo(15) << std::endl;      // <2>

    return 0;
}


