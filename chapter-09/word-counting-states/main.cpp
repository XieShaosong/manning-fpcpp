#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <variant>

#include <cassert>

// A helper to create overloaded function objects
template <typename... Fs>
struct overloaded : Fs... { using Fs::operator()...; };

template <typename... Fs> overloaded(Fs...) -> overloaded<Fs...>;


class program_t {
private:
    // The initial state does not need to contain anything
    class init_t {
    };

    // The running state contains the current count and the file
    // that we are reading from
    class running_t {
    public:
        running_t(const std::string& file_name)
            : m_file(file_name)
        {
        }

        void count_words()
        {
            m_count = std::distance(
                    std::istream_iterator<std::string>(m_file),
                    std::istream_iterator<std::string>());
        }

        unsigned count() const
        {
            return m_count;
        }

    private:
        unsigned m_count = 0;
        std::ifstream m_file;
    };

    // The finished state contains only the final count
    class finished_t {
    public:
        finished_t(unsigned count = 0)
            : m_count(count)
        {
        }

        unsigned count() const
        {
            return m_count;
        }

    private:
        unsigned m_count;

    };

    std::variant < init_t
                 , running_t
                 , finished_t
                 > m_state;


public:
    program_t()
        : m_state(init_t())
    {
    }

    void count_words(const std::string& file_name)
    {
        assert(m_state.index() == 0);

        m_state = running_t(file_name);

        std::get<running_t>(m_state).count_words();

        counting_finished();
    }

    void counting_finished()
    {
        // One of the ways to work with variants (section 9.1.2) is
        // to use the std::get_if function which returns a pointer
        // to the data stored in the variant if it contains a value
        // of the requested type. Otherwise, it returns nullptr
        const auto *state = std::get_if<running_t>(&m_state);

        assert(state != nullptr);

        m_state = finished_t(state->count());
    }

    unsigned count() const
    {
        // Another approach is to use the std::visit function
        // which executes a given function on the value stored
        // inside of the variant.
        //
        // The `overloaded` helper function can be used to combine
        // several lambdas of different signatures into a single function
        // object
        return std::visit(overloaded {
                    [] (init_t) {
                        return (unsigned)0;
                    },
                    [] (const running_t& state) {
                        return state.count();
                    },
                    [] (const finished_t& state) {
                        return state.count();
                    }
                }, m_state);
    }
};


int main(int argc, char *argv[])
{
    program_t program;
    program.count_words("main.cpp");

    std::cout << program.count() << std::endl;
}

