
#include <iostream>
#include <vector>
#include <algorithm>

#include "../../common/person.h"

// Implements a generic function object that compares an object's age
// against a predefined integer limit (section 3.1.4)
class older_than {
public:
    older_than(int limit)
       : m_limit(limit)
    {
    }

    // Defining a call operator as a template function,
    // we will create a generic function object
    // that deduces the type of its argument when invoked
    template <typename T>
    bool operator() (T &&object) const
    {
        return std::forward<T>(object).age() > m_limit;
    }

private:
    int m_limit;
};


int main(int argc, char *argv[])
{
    std::vector<person_t> persons;

    older_than predicate(42);

    std::count_if(persons.cbegin(), persons.cend(), predicate);

    return 0;
}
