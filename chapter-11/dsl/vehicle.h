/*
 * vehicle.h
 * Copyright (C) 2017 Ivan Čukić <ivan.cukic(at)kde.org>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

#include "dsl.h"

namespace vehicle {
    class record {
    public:
        record() = default;

        record(std::string model, std::string color)
            : m_model(model)
            , m_color(color)
        {
        }

        std::string model() const
        {
            return m_model;
        }

        std::string color() const
        {
            return m_color;
        }

        bool set_model(const std::string& model)
        {
            if (model.empty()) return false;

            m_model = model;
            return true;
        }

        bool set_color(const std::string& color)
        {
            if (color.empty()) return false;

            m_color = color;
            return true;
        }

    private:
        std::string m_model;
        std::string m_color;
    };

    auto model = field { &record::set_model };
    auto color = field { &record::set_color };
} // namespace vehicle


#endif /* !VEHICLE_H */
