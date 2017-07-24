
#include <functional>
#include <string>
#include <iostream>
#include <mutex>
#include <optional>

template <typename F>
class lazy_val {
private:
    F m_computation;

    // In the book, we are using a value and a Boolean flag to denote
    // whether we have already calculated the value or not. In this
    // implementation, we are using std::optional which is like a container
    // that can either be empty or it can contain a single value --
    // exactly what we tried to simulate with the value/Boolean flag pair.
    // Optional values will be coveref in more detail in chapter 9.
    mutable std::optional<decltype(m_computation())> m_value;
    mutable std::mutex m_value_lock;

public:
    lazy_val(F function)
        : m_computation(function)
    {
    }

    lazy_val(lazy_val &&other)
        : m_computation(std::move(other.m_computation))
    {
    }

    operator decltype(m_computation()) () const
    {
        std::lock_guard<std::mutex> lock(m_value_lock);

        if (!m_value) {
            m_value = std::invoke(m_computation);
        }

        return *m_value;
    }

};

template <typename F>
lazy_val<F> make_lazy_val(F &&function)
{
    return lazy_val<F>(std::forward<F>(function));
}


// Instead of the make_lazy_val_helper function, we can define the `lazy`
// keyword with a bit of macro trickery - the macro will call the operator
// minus and create the lambda head - we can only provide the lambda
// body when creating the lazy value

struct _make_lazy_val_helper {
    template <typename F>
    auto operator - (F &&function) const
    {
        return lazy_val<F>(function);
    }
} make_lazy_val_helper;

#define lazy make_lazy_val_helper - [=]


int main(int argc, char *argv[])
{
    int number = 6;

    auto val = lazy {
        std::cout << "Calculating the answer..." << std::endl;
        std::cout << "while the number is: " << number << std::endl;
        return 42;
    };

    number = 2;

    std::cout << "Lazy value defined" << std::endl;

    std::cout << val << std::endl;
}

