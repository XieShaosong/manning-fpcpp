/*
 * person.h
 * Copyright (C) 2017 Ivan Čukić <ivan.cukic(at)kde.org>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PERSON_H
#define PERSON_H

#include <string>

#include "dsl.h"

namespace person {
    struct record {
        std::string name;
        std::string surname;
        unsigned int age;
    };

    auto name    = field { &record::name };
    auto surname = field { &record::surname };
    auto age     = field { &record::age };
} // namespace person


#endif /* !PERSON_H */
