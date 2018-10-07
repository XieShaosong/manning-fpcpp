
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

template <typename C, typename T = typename C::value_type>
std::unordered_map<T, unsigned int> count_occurences(
        const C &collection)
{
    std::unordered_map<T, unsigned int> result;

    for (const auto &c: collection) {
        result[c]++;
    }

    return result;
}


// Reverses all pairs in a collection (section 4.4.1)
template <
    typename C,
    typename P1 = typename std::remove_cv<
             typename C::value_type::first_type>::type,
    typename P2 = typename C::value_type::second_type
    >
std::vector<std::pair<P2, P1>> reverse_pairs(const C &items)
{
    std::vector<std::pair<P2, P1>> result(items.size());

    // For each pair (first, second) in the source collection,
    // put (second, first) into the destination collection
    std::transform(
        std::begin(items), std::end(items),
        std::begin(result),
        [] (const std::pair<const P1, P2> &p)
        {
            return std::make_pair(p.second, p.first);
        }
    );

    return result;
}


int main(int argc, char *argv[])
{
    std::string sentence = "Hello world";
    std::vector<std::string> words { std::string("The"),
        std::string("Farm"),
        std::string("from"),
        std::string("the"),
        std::string("Animal"),
        std::string("Farm")
    };

    for (const auto& item: reverse_pairs(count_occurences(sentence))) {
        std::cout << item.first << " " << item.second << std::endl;
    }

    for (const auto& item: reverse_pairs(count_occurences(words))) {
        std::cout << item.first << " " << item.second << std::endl;
    }

    return 0;
}
