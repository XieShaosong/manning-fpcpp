#ifndef SERVICE_H
#define SERVICE_H

// Standard library
#include <iostream>
#include <functional>

// We are faking the reactive stream with a range
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/to_container.hpp>

/**
 * A structure to contain a value of any type,
 * and a contextual information - the message
 * we are expecting the server to reply with
 */
template <typename MessageType>
struct with_expected_reply {
    MessageType value;
    std::string expected_reply;

    void reply(const std::string& message) const
    {
        REQUIRE(message == expected_reply);
    }
};

/**
 * Function that constructs an instance of with_expected_reply given a
 * value and a socket
 */
template <typename MessageType>
auto make_with_expected_reply(MessageType&& value, const std::string& expected_reply)
{
    return with_expected_reply<MessageType>{
        std::forward<MessageType>(value), expected_reply};
}

/**
 * Lift any function from T1 to T2 to a function
 * from with_expected_reply<T1> to with_expected_reply<T2>
 */
template <typename F>
auto lift_with_expected_reply(F&& function)
{
    return [function = std::forward<F>(function)] (auto &&ws) {
        return make_with_expected_reply(std::invoke(function, ws.value), ws.expected_reply);
    };
}

/**
 * Converts a function from T1 to T2 to a
 * function from with_expected_reply<T1> to T2.
 * This is not a given thing for all monads,
 * but we need it to be able to use the filter
 * transformation on streams that emit values
 * with the expected reply
 */
template <typename F>
auto apply_with_expected_reply(F&& function)
{
    return [function = std::forward<F>(function)] (auto &&ws) {
        return std::invoke(function, std::forward<decltype(ws)>(ws).value);
    };
}

#endif
