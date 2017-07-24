
#include <iostream>
#include <string>

#include "dsl.h"
#include "person.h"
#include "vehicle.h"

int main(int argc, char *argv[])
{
    {
        using namespace person;

        record martha;

        with(martha) (
                name = "Martha",
                surname = "Jones"
            );

        std::cout << martha.name << std::endl;
    }

    {
        using namespace vehicle;

        record car;

        with(car) (
                model = "Ford T",
                color = "blue"
            );

        std::cout << car.model() << " " << car.color() << std::endl;

        with(car) (
                model = "Ford T",
                color = ""
            );

        std::cout << car.model() << " " << car.color() << std::endl;
    }


    return 0;
}





