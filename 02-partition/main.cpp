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

// tag::book_partition[]
    std::partition(
            people.begin(), people.end(),
            is_female
        );
// end::book_partition[]

    for (const auto& person: people) {
        std::cout << name(person) << '\n';
    }

    return 0;
}
