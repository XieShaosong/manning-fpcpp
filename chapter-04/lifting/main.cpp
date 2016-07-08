#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <fstream>

// tag::pointer_lift[]
template <typename Function>
auto pointer_lift(Function f)
{
    return [f] (auto *item) {
        if (item) {
            f(*item);
        }
    };
}
// end::pointer_lift[]

// tag::collection_lift[]
template <typename Function>
auto collection_lift(Function f)
{
    return [f] (auto &items) {
        for (auto &item: items) {
            f(item);
        }
    };
}
// end::collection_lift[]

// tag::non_member_lambda[]
// end::non_member_lambda[]

void to_upper(std::string &string)
{
    for (auto &c: string) c = toupper(c);
}

int main(int argc, char *argv[])
{
    auto pointer_to_upper = pointer_lift(to_upper);
    auto vector_to_upper  = collection_lift(to_upper);

    std::string string           = "Hello World";
    std::string *string_ptr      = &string;
    std::string *string_ptr_null = nullptr;

    std::vector<std::string> string_vector { string, string };
    std::vector<std::string> string_vector_empty {};

    to_upper(string);

    pointer_to_upper(string_ptr);
    pointer_to_upper(string_ptr_null);

    vector_to_upper(string_vector);
    vector_to_upper(string_vector_empty);


    std::cout << string << std::endl;
    std::cout << *string_ptr << std::endl;
    std::cout << string_ptr_null << std::endl;

    for (const auto& item: string_vector) {
        std::cout << item << std::endl;
    }

    for (const auto& item: string_vector_empty) {
        std::cout << item << std::endl;
    }

    return 0;
}
