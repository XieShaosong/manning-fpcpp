// vim: set undofile:
// Program: 06-string-concatenation-refs

#include <iostream>
#include <string>
#include <tuple>

template <typename... Strings>
class lazy_string_concat_helper;

template <typename LastString, typename... Strings>
class lazy_string_concat_helper<LastString,
                                Strings...> {
private:
    const LastString& data;
    lazy_string_concat_helper<Strings...> tail;

public:
    lazy_string_concat_helper(
            const LastString& data,
            lazy_string_concat_helper<Strings...> tail)
        : data(data)
        , tail(tail)
    {
    }

    int size() const
    {
        return data.size() + tail.size();
    }

    template <typename It>
    void save(It end) const
    {
        const auto begin = end - data.size();
        std::copy(data.cbegin(), data.cend(),
                  begin);
        tail.save(begin);
    }

    operator std::string() const
    {
        std::string result(size(), '\0');
        save(result.end());
        return result;
    }

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


template <typename Stream, typename... Strings>
Stream& operator<<(Stream& stream,
                   const lazy_string_concat_helper<Strings...>& strings)
{
    return stream << static_cast<std::string>(strings);
}


lazy_string_concat_helper<> lazy_concat;

int main(int argc, char* argv[])
{
    // When using `auto` and expression templates that store references
    // to values that can change over time, there can be some unexpected
    // consequences.
    std::string name = "Jane";
    std::string surname = "Smith";

    const auto fullname =
        lazy_concat + surname + ", " + name;

    name = "John";

    // The fullname is defined before the `name` variable was changed,
    // but the concatenation is performed after the change
    std::cout << fullname << std::endl;
}
