#ifndef TRIM_H
#define TRIM_H

#include <string>
#include <algorithm>
#include <locale>

namespace detail {
    bool is_not_space(char c)
    {
        return !std::isspace(c, std::locale());
    }
}

std::string trim_left(std::string s)
{
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(), detail::is_not_space));
    return s;
}

std::string trim_right(std::string s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), detail::is_not_space).base(),
            s.end());
    return s;
}

std::string trim(const std::string& s)
{
    return trim_left(trim_right(s));
}

#endif
