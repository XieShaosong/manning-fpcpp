#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <fstream>

#include "../../common/person.h"

// tag::print_person[]
void print_person(const person_t &person,
                  std::ostream &out,
                  person_t::output_format_t format)
{
    if (format == person_t::name_only) {
        out << person.name() << '\n';

    } else if (format == person_t::full_name) {
        out << person.name() << ' '
            << person.surname() << '\n';

    }
}
// end::print_person[]

int main(int argc, char *argv[])
{
    using namespace std::placeholders;

    std::vector<person_t> people {
        { "David"  , person_t::male   },
        { "Jane"   , person_t::female },
        { "Martha" , person_t::female },
        { "Peter"  , person_t::male   },
        { "Rose"   , person_t::female },
        { "Tom"    , person_t::male   }
    };

    std::ofstream file("test");

// tag::non_member[]
    std::for_each(people.cbegin(), people.cend(),
            std::bind(print_person,
                      _1,
                      std::ref(std::cout),
                      person_t::name_only
                ));

    std::for_each(people.cbegin(), people.cend(),
            std::bind(print_person,
                      _1,
                      std::ref(file),
                      person_t::full_name
                ));
// end::non_member[]

// tag::member[]
    std::for_each(people.cbegin(), people.cend(),
            std::bind(&person_t::print,
                      _1,
                      std::ref(std::cout),
                      person_t::name_only
                ));
// end::member[]

// tag::non_member_lambda[]
    std::for_each(people.cbegin(), people.cend(),
            [] (const person_t &person) {
                print_person(person,
                             std::cout,
                             person_t::name_only);
            });

    std::for_each(people.cbegin(), people.cend(),
            [&file] (const person_t &person) {
                print_person(person,
                             file,
                             person_t::full_name);
            });
// end::non_member_lambda[]

    return 0;
}
