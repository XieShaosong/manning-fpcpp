#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include "../common/person.h"

std::string name(const person_t &person)
{
    return person.name();
}

bool is_female(const person_t &person)
{
    return person.gender() == person_t::female;
}

bool is_not_female(const person_t &person)
{
    return !is_female(person);
}

// These functions are inefficient, they just
// exist to demonstrate the recursive implementation
// of the names_for function.

template <typename T>
T tail(const T &collection)
{
    return T(collection.cbegin() + 1, collection.cend());
}

template <typename T, typename C>
C prepend(T &&item, C collection)
{
    C result(collection.size() + 1);
    result[0] = std::forward<T>(item);
    std::copy(collection.cbegin(), collection.cend(), result.begin() + 1);
    return result;
}


/*
// tag::book_signature[]
template <typename FilterFunction>
std::vector<std::string> names_for(
        const std::vector<person_t> &people,
        FilterFunction filter)
// end::book_signature[]
*/

// #define USE_LOOP_IMPLEMENTATION
// #define USE_RECURSIVE_IMPLEMENTATION
#define USE_TAIL_RECURSIVE_IMPLEMENTATION

#ifdef USE_LOOP_IMPLEMENTATION
// tag::book_loop[]
template <typename FilterFunction>
std::vector<std::string> names_for(
        const std::vector<person_t> &people,
        FilterFunction filter)
{
    std::vector<std::string> result;

    for (const person_t& person: people) {
        if (filter(person)) {
            result.push_back(name(person));
        }
    }

    return result;
}
// end::book_loop[]
#endif

#ifdef USE_RECURSIVE_IMPLEMENTATION
// tag::book_recursive[]
template <typename FilterFunction>
std::vector<std::string> names_for(
        const std::vector<person_t> &people,
        FilterFunction filter)
{
    if (people.empty()) {
        return {};

    } else {
        const auto head = people.front();
        const auto processed_tail = names_for(
                tail(people),
                filter);

        if (filter(head)) {
            return prepend(name(head), processed_tail);
        } else {
            return processed_tail;
        }
    }
}
// end::book_recursive[]
#endif

#ifdef USE_TAIL_RECURSIVE_IMPLEMENTATION
// tag::book_tailrecursive_helper[]
template <typename FilterFunction, typename Iterator>
std::vector<std::string> names_for_helper(
        Iterator people_begin,
        Iterator people_end,
        FilterFunction filter,
        std::vector<std::string> previously_collected)
{
    if (people_begin == people_end) {
        return previously_collected;

    } else {
        const auto head = *people_begin;

        if (filter(head)) {
            previously_collected.push_back(name(head));
            return names_for_helper(
                    people_begin + 1,
                    people_end,
                    filter,
                    previously_collected);

        } else {
            return names_for_helper(
                    people_begin + 1,
                    people_end,
                    filter,
                    previously_collected);
        }
    }
}
// end::book_tailrecursive_helper[]
// tag::book_tailrecursive[]
template <typename FilterFunction, typename Iterator>
std::vector<std::string> names_for(
        Iterator people_begin,
        Iterator people_end,
        FilterFunction filter)
{
        return names_for_helper(people_begin,
                                people_end,
                                filter,
                                {});
}
// end::book_tailrecursive[]
#endif


int main(int argc, char *argv[])
{
    std::vector<person_t> people {
        { "David"  , person_t::male   },
        { "Jane"   , person_t::female },
        { "Martha" , person_t::female },
        { "Peter"  , person_t::male   },
        { "Rose"   , person_t::female },
        { "Tom"    , person_t::male   }
    };

#ifdef USE_TAIL_RECURSIVE_IMPLEMENTATION
    auto names = names_for(people.begin(), people.end(), is_female);
#else
    auto names = names_for(people, is_female);
#endif

    for (const auto& name: names) {
        std::cout << name << '\n';
    }

    return 0;
}
