// vim: set undofile:
// Program: 06-string-concatenation

#include <iostream>
#include <string>
#include <tuple>

// Using expression templates to concatenate strings

template <typename... Strings>
class lazy_string_concat_helper;

template <typename LastString, typename... Strings>
class lazy_string_concat_helper<LastString,
                                Strings...> {
private:
    // Stores the copy of the original string
    LastString data;

    // Stores the structure that contains other strings
    lazy_string_concat_helper<Strings...> tail;

public:
    lazy_string_concat_helper(
            LastString data,
            lazy_string_concat_helper<Strings...> tail)
        : data(data)
        , tail(tail)
    {
    }

    // Calculates the size of all strings combined
    int size() const
    {
        return data.size() + tail.size();
    }

    // The structure stores strings in reverse order: the data
    // member variable contains the string that comes last,
    // so it needs to go to the end of the buffer.
    template <typename It>
    void save(It end) const
    {
        const auto begin = end - data.size();
        std::copy(data.cbegin(), data.cend(),
                  begin);
        tail.save(begin);
    }

    // When you want to convert the expression definition into a real string,
    // allocate enough memory and start copying the strings into it.
    operator std::string() const
    {
        std::string result(size(), '\0');
        save(result.end());
        return result;
    }

    // Creates a new instance of the structure with one string added to it
    lazy_string_concat_helper<std::string,
                              LastString,
                              Strings...>
    operator+(const std::string& other) const
    {
        return lazy_string_concat_helper
               <std::string, LastString, Strings...>(
                   other,
                   *this
               );
    }
};



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


lazy_string_concat_helper<> lazy_concat;

int main(int argc, char* argv[])
{
    std::string name = "Jane";
    std::string surname = "Smith";

    // You can’t overload the operator+ on std::string, so use a small
    // trick to force the use of the concatenation structure by appending
    // to an instance of it.
    const std::string fullname =
        lazy_concat + surname + ", " + name;

    std::cout << fullname << std::endl;
}

