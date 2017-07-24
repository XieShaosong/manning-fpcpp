/*
 * dsl.h
 * Copyright (C) 2017 Ivan Čukić <ivan.cukic(at)kde.org>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef DSL_H
#define DSL_H

#include <type_traits>
#include <functional>

template <typename Member, typename Value>
struct update {
    update(Member member, Value value)
        : member(member)
        , value(value)
    {
    }

    template <typename Record>
    bool operator() (Record& record)
    {
        if constexpr (std::is_invocable_r<bool, Member, Record, Value>()) {
            return std::invoke(member, record, value);

        } else if constexpr (std::is_invocable<Member, Record, Value>()) {
            std::invoke(member, record, value);
            return true;

        } else {
            std::invoke(member, record) = value;
            return true;
        }
    }

    Member member;
    Value value;
};


template <typename Member>
struct field {
    field(Member member)
        : member(member)
    {
    }

    template <typename Value>
    auto operator= (Value value) const
    {
        return update { member, value };
    }

    Member member;
};




template <typename Record>
class transaction {
public:
    transaction(Record& record)
        : m_record(record)
    {
    }

    template <typename ...Updates>
    bool operator() (Updates ...updates)
    {
        auto temp = m_record;

        if (all( updates(temp)... )) {
            std::swap(m_record, temp);
            return true;
        }

        return false;
    }

private:
    template <typename ...Updates>
    bool all(Updates... results) const
    {
        return (... && results);
    }

    Record &m_record;
};

template <typename Record>
auto with(Record& record)
{
    return transaction(record);
}







#endif /* !DSL_H */
