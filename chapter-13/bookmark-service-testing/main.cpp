
// Standard library
#include <iostream>
#include <string>

// JSON library
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// Catch library
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

// Utilities
#include "expected.h"
#include "mtry.h"
#include "trim.h"

// Service implementation
#include "service.h"

/**
 * For funcitons that return json objects, but which can fail
 * and return an exception
 */
using expected_json = expected<json, std::exception_ptr>;

/**
 * Basic bookmark information. Holds an URL and a title
 */
struct bookmark_t {
    std::string url;
    std::string text;
};

/**
 * Creates a string from the bookmark info in the following format:
 * [text](url)
 */
std::string to_string(const bookmark_t& page)
{
    return "[" + page.text + "](" + page.url + ")";
}

/**
 * Writes the bookmark info in the following format:
 * [text](url)
 */
std::ostream& operator<<(std::ostream& out, const bookmark_t& page)
{
    return out << "[" << page.text << "](" << page.url << ")";
}

/**
 * Type that contains a bookmark or an error (an exception)
 */
using expected_bookmark = expected<bookmark_t, std::exception_ptr>;

/**
 * Tries to read the bookmark info from JSON. In the case
 * of failure, it will return an exception.
 */
expected_bookmark bookmark_from_json(const json& data)
{
    return mtry([&] {
            return bookmark_t { data.at("FirstURL"), data.at("Text") };
        });
}

TEST_CASE("Counting newlines in a string", "[program-logic]")
{
    using namespace ranges::v3;

    // We are lifting the transform and filter functions
    // to work on the with_expected_reply<T> type that adds the
    // socket information to the given value so that we
    // can reply to the client
    auto transform = [](auto f) {
            return view::transform(lift_with_expected_reply(f));
        };
    auto filter = [](auto f) {
            return view::filter(apply_with_expected_reply(f));
        };

    // The sink concept does not exist for ranges,
    // but we can just use view::transform to force the
    // function f to be evaluated for all values
    // in the range that we are sending to the sink
    auto sink = [](auto f) {
            return view::transform([f] (auto &&ws) {
                    f(ws);
                    return ws.expected_reply;
                });
        };


    // Pretty function to create test cases
    auto test = [] (const std::string& message, const std::string& expected_reply) {
        return with_expected_reply<std::string>{message, expected_reply};
        };

    std::vector<with_expected_reply<std::string>> source {
        test("", ""),

        test("hello",
             "ERROR: Request not understood\n"),

        test("{}",
             "ERROR: Request not understood\n"),

        test("{\"FirstURL\" : \"http://www.iso.org/\",\"Text\" : \"ISO\"}",
             "ERROR: Not a C++-related link\n"),

        test("{\"FirstURL\" : \"http://isocpp.org/\",\"Text\" : \"ISO C++ -- Official site\"}",
             "OK: [ISO C++ -- Official site](http://isocpp.org/)\n")
    };

    auto pipeline =
        source
            | transform(trim)

            // Ignoring comments and empty messages
            | filter([] (const std::string &message) {
                return message.length() > 0 && message[0] != '#';
            })

            // Trying to parse the input
            | transform([] (const std::string &message) {
                return mtry([&] {
                    return json::parse(message);
                });
            })

            // Converting the result into the bookmark
            | transform([] (const auto& exp) {
                return mbind(exp, bookmark_from_json);
            })

            | sink([] (const auto &message) {
                const auto exp_bookmark = message.value;

                if (!exp_bookmark) {
                    message.reply("ERROR: Request not understood\n");
                    return;
                }

                if (exp_bookmark->text.find("C++") != std::string::npos) {
                    message.reply("OK: " + to_string(exp_bookmark.get()) + "\n");
                } else {
                    message.reply("ERROR: Not a C++-related link\n");
                }
            });

    // Forcing the range to evaluate its elements
    pipeline | to_vector;
}
