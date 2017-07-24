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
    class init_t {
    };

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
        const auto *state = std::get_if<running_t>(&m_state);

        assert(state != nullptr);

        m_state = finished_t(state->count());
    }

    unsigned count() const
    {
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

