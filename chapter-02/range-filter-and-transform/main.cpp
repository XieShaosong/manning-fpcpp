#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include <range/v3/view/transform.hpp>
#include <range/v3/view/filter.hpp>

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

int main(int argc, char *argv[])
{
    using ranges::v3::view::filter;
    using ranges::v3::view::transform;

    std::vector<person_t> people {
        { "David"  , person_t::male   },
        { "Jane"   , person_t::female },
        { "Martha" , person_t::female },
        { "Peter"  , person_t::male   },
        { "Rose"   , person_t::female },
        { "Tom"    , person_t::male   }
    };

// tag::book[]

    auto names = transform(filter(people, is_female), name);

// end::book[]

    for (const auto& value: names) {
        std::cout << value << '\n';
    }

// tag::book_pipe[]

    for (const auto& value: people | filter(is_female) | transform(name)) {
        std::cout << value << '\n';
    }

// end::book_pipe[]

    return 0;
}
