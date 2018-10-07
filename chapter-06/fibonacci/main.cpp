// vim: set undofile:
// Program: 06-fibonacci

#include <iostream>
#include <vector>

// Uncomment one of these
// #define ORDINARY_FIB
// #define CACHED_FIB
#define OPTIMIZED_CACHED_FIB


#ifdef ORDINARY_FIB
// Ordinary recursive implementation of the Fibonacci function.
// This implementation is extremely slow -- O(2^n) complexity
unsigned int fib(unsigned int n)
{
    return n == 0 ? 0 :
           n == 1 ? 1 :
                    fib(n - 1) + fib(n -2);
}
#endif


#ifdef CACHED_FIB
std::vector<unsigned int> cache{0, 1};

// Implementation of the Fibonacci function which caches all
// previously calculated results
unsigned int fib(unsigned int n)
{
    if (cache.size() > n) {
        std::cerr << "using cache " << n << std::endl;
        return cache[n];

    } else {
        const auto result = fib(n - 1) + fib(n - 2);
        cache.push_back(result);
        return result;
    }
}
#endif


#ifdef OPTIMIZED_CACHED_FIB

// In order to calculate Fibonacci numbers, we just need to cache the
// last two results
class fib_cache {
public:
    fib_cache()
        : m_previous{0}
        , m_last{1}
        , m_size{2}
    {
    }

    size_t size() const
    {
        return m_size;
    }

    unsigned int operator[](unsigned int n) const
    {
        return n == m_size - 1 ? m_last :
               n == m_size - 2 ? m_previous :
               throw std::out_of_range("The value is no longer in the cache");
    }

    void push_back(unsigned int value)
    {
        m_size++;
        m_previous = m_last;
        m_last = value;
    }

private:
    unsigned int m_previous;
    unsigned int m_last;
    size_t m_size;

} cache;

unsigned int fib(unsigned int n)
{
    if (cache.size() > n) {
        return cache[n];

    } else {
        const auto result = fib(n - 1) + fib(n - 2);
        cache.push_back(result);
        return result;
    }
}
#endif


int main(int argc, char* argv[])
{
    for (int i = 0; i < 10; ++i) {
        std::cout << fib(i) << std::endl;
    }

    return 0;
}
