#include <ratio>


template < typename Representation
         , typename Ratio = std::ratio<1>
         >
class distance {
public:
    explicit constexpr distance(Representation value)
        : value(value)
    {
    }

    distance operator+ (const distance &other) const
    {
        return distance(value + other.value);
    }

    Representation value;
};



template <typename Representation>
using meters = distance<Representation>;

template <typename Representation>
using kilometers = distance<Representation, std::kilo>;

template <typename Representation>
using centimeters = distance<Representation, std::centi>;

template <typename Representation>
using miles = distance<Representation, std::ratio<1609>>;


namespace distance_literals {

    constexpr kilometers<long double> operator ""_km(long double distance)
    {
        return kilometers<long double>(distance);
    }

    constexpr miles<long double> operator ""_mi(long double distance)
    {
        return miles<long double>(distance);
    }

}



int main(int argc, char *argv[])
{
    using namespace distance_literals;

    auto distance_in_kilometers = 42.0_km;
    auto distance_in_miles      = 1.0_mi;

    // Ok:
    auto more_distance = distance_in_kilometers + distance_in_kilometers;

    // Error: operator+ is not defined for these types
    // auto total_distance = distance_in_kilometers + distance_in_miles;

    return 0;
}

