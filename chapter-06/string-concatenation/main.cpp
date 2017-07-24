// vim: set undofile:
// Program: 06-string-concatenation

#include <iostream>
#include <string>
#include <tuple>

// tag::book_struct[]
template <typename... Strings>
class lazy_string_concat_helper;

template <typename LastString, typename... Strings>
class lazy_string_concat_helper<LastString,
                                Strings...> {
private:
    LastString data;                                      // <1>
    lazy_string_concat_helper<Strings...> tail;           // <2>

public:
    lazy_string_concat_helper(
            LastString data,
            lazy_string_concat_helper<Strings...> tail)
        : data(data)
        , tail(tail)
    {
    }

    int size() const                                      // <3>
    {                                                     // <3>
        return data.size() + tail.size();                 // <3>
    }                                                     // <3>

    template <typename It>
    void save(It end) const
    {                                                     // <4>
        const auto begin = end - data.size();             // <4>
        std::copy(data.cbegin(), data.cend(),             // <4>
                  begin);                                 // <4>
        tail.save(begin);                                 // <4>
    }

    operator std::string() const
    {
        std::string result(size(), '\0');                 // <5>
        save(result.end());                               // <5>
        return result;                                    // <5>
    }

    lazy_string_concat_helper<std::string,
                              LastString,
                              Strings...>
    operator+(const std::string& other) const             // <6>
    {                                                     // <6>
        return lazy_string_concat_helper                  // <6>
               <std::string, LastString, Strings...>(     // <6>
                   other,                                 // <6>
                   *this                                  // <6>
               );                                         // <6>
    }
};
// end::book_struct[]


// tag::book_struct_base[]
template <>
class lazy_string_concat_helper<> {
public:
    lazy_string_concat_helper()
    {
    }

    int size() const
    {
        return 0;
    }

    template <typename It>
    void save(It) const
    {
    }

    lazy_string_concat_helper<std::string>
    operator+(const std::string& other) const
    {
        return lazy_string_concat_helper<std::string>(
                other,
                *this
            );
    }
};
// end::book_struct_base[]

// tag::book_struct_demo[]
lazy_string_concat_helper<> lazy_concat;           // <1>

int main(int argc, char* argv[])
{
    std::string name = "Jane";
    std::string surname = "Smith";

    const std::string fullname =                   // <1>
        lazy_concat + surname + ", " + name;       // <1>

    std::cout << fullname << std::endl;
}
// end::book_struct_demo[]
