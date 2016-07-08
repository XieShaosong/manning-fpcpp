
#include <iostream>
#include <vector>
#include <algorithm>

#include "../../common/person.h"


class company_t {
public:
    std::string team_name_for(const person_t &) const;

    int count_team_members(const std::string &team_name) const;

private:
    std::vector<person_t> m_employees;

};

std::string company_t::team_name_for(const person_t &person) const
{
    // Just for testing, you can implement this
    // properly for exercise
    return "Team1";
}

int company_t::count_team_members(const std::string &team_name) const
{
    return std::count_if(
            m_employees.cbegin(), m_employees.cend(),
            [this, &team_name]
                (const person_t &employee)
            {
                return team_name_for(employee) == team_name;
            }
        );
}

int main(int argc, char *argv[])
{
    // Also for exercise, create an instance of the company_t object,
    // and test the count_team_members member function
}

