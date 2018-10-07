
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

    // Filtering the collection by copying (see section 2.2.5)
    std::vector<person_t> females;

    // The std::copy_if algorithm copies items that satisfy the
    // is_female predicate into the destination collection
    std::copy_if(people.cbegin(), people.cend(),
                 std::back_inserter(females),
                 is_female);

    // Transforming to get the names
    std::vector<std::string> names(females.size());

    std::transform(females.cbegin(), females.cend(),
                   names.begin(),
                   name);

    for (const auto& name: names) {
        std::cout << name << '\n';
    }

    return 0;
}
