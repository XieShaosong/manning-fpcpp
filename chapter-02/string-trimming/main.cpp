
#include <iostream>
#include <algorithm>
#include <string>

bool is_not_space(char c)
{
    return !std::isspace(c, std::locale());
}

// tag::book_ltrim[]
std::string trim_left(std::string s)
{
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(), is_not_space));
    return s;
}
// end::book_ltrim[]

// tag::book_rtrim[]
std::string trim_right(std::string s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), is_not_space).base(),
            s.end());
    return s;
}
// end::book_rtrim[]

// tag::book_trim[]
std::string trim(const std::string &s)
{
    return trim_left(trim_right(s));
}
// end::book_trim[]

int main(int argc, char *argv[])
{
    std::cout << '[' << trim_left("    Fun with functions    ")  << "]\n";
    std::cout << '[' << trim_right("    Fun with functions    ") << "]\n";
    std::cout << '[' << trim("    Fun with functions    ")       << "]\n";

    return 0;
}
