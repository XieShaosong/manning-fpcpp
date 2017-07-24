#include <iostream>
#include <algorithm>
#include <numeric>

#include <vector>
#include <string>
#include <type_traits>

#include "type_utils.h"

// Function that sums all items in an iterable collection
template < typename C
         , typename R = contained_type<C>
         >
R sum_iterable(const C &collection)
{
    std::cout << "This is sum_iterable\n";
    return std::accumulate(
            begin(collection),
            end(collection),
            R());
}


// Function that sums all items in a collection
// that has a nested value_type definition
template < typename C
         , typename R = typename C::value_type
         >
R sum_collection(const C &collection)
{
    std::cout << "This is sum_collection\n";
    return std::accumulate(
            begin(collection),
            end(collection),
            R());
}



// Function that sums all items in a collection
// which first tries to use C::value_type and then
// contained_type<C>
template <typename C>
auto sum(const C &collection)
{
    if constexpr (has_value_type<C>()) {
        return sum_collection(collection);
    } else if constexpr (is_iterable<C>()) {
        return sum_iterable(collection);
    } else {
        static_assert(false_<C>(), "sum can be called only on collections");
    }
}


// Meta-function that checks whether two types
// are identical
template <typename T1, typename T2>
struct is_same: std::false_type {};

template <typename T>
struct is_same<T, T>: std::true_type {};


int main(int argc, char *argv[])
{
    // Uncomment this to make the compiler write the
    // exact result of the contained_type meta-function
    // error<contained_type<std::vector<std::string>>>();

    // Asserting that std::vector<std::string> contains
    // values of type std::string
    static_assert(
        is_same<
            contained_type<std::vector<std::string>>,
            std::string
        >(), "Expected the contained_type to return std::string");

    // Calling the function sum on a vector of ints.
    // The result will be an int
    std::vector<int> xs { 1, 2, 3, 4 };
    std::cout << sum_iterable(xs) << std::endl;
    std::cout << sum_collection(xs) << std::endl;
    std::cout << sum(xs) << std::endl;

    // Uncomment this to trigger the static_assert in the
    // sum function
    // sum(1);

    return 0;
}
