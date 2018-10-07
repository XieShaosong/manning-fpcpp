
#include <functional>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

template < typename BeginIt
         , typename EndIt
         , typename MidIt
         >
void sort_until(BeginIt begin, MidIt mid, EndIt end)
{
    // The `std::nth_element` algorithm is a partial sorting algorithm.
    //
    // After the algorithm is executed, the element at the `mid` position
    // will be the same element which would be at that position if
    // the collection was fully sorted.
    std::nth_element(begin, mid, end);

    // It also partitions the collection to have all of the elements before
    // this new `mid` element are less than or equal to the elements after
    // the new `mid` element.

    // This means that the elements before `mid` are the elements that
    // would be before `mid` in the completely sorted collection.
    // We just need to sort them.
    std::sort(begin, mid);
}



int main(int argc, char *argv[])
{
    std::vector<int> xs { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(xs.begin(), xs.end(), g);

    std::cout << "Before sorting: ";
    for (const auto& x: xs) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;

    sort_until(xs.begin(), xs.begin() + 3, xs.end());

    std::cout << "After sorting: ";
    for (const auto& x: xs) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;
}

