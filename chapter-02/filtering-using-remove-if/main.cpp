
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include "../../common/person.h"

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

    // Filtering with the erase-remove idiom (section 2.2.5, page 35)
    people.erase(
        std::remove_if(people.begin(), people.end(),
                       is_not_female),
        people.end());

    for (const person_t& person: people) {
        std::cout << person.name() << '\n';
    }

    return 0;
}
