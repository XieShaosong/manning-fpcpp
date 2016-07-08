
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include "../../common/person.h"

typedef std::pair<std::string, bool> list_item;

std::string title(const list_item &item)
{
    return item.first + std::string(item.second, '*');
}

bool is_selected(const list_item &item)
{
    return item.second;
}

bool is_not_selected(const list_item &item)
{
    return !item.second;
}

template <typename It>
void move_selected_to(It first, It last, It destination)
{
    std::stable_partition(first, destination, is_not_selected);
    std::stable_partition(destination, last,  is_selected);
}

int main(int argc, char *argv[])
{
    std::vector<list_item> people {
        { "David"  , true },
        { "Jane"   , false },
        { "Martha" , false },
        { "Peter"  , false },
        { "Rose"   , true },
        { "Tom"    , true }
    };

    move_selected_to(people.begin(), people.end(), people.begin() + 3);

    for (const auto& person: people) {
        std::cout << title(person) << '\n';
    }

    return 0;
}
