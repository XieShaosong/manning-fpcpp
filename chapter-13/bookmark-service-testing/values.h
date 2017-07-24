#ifndef VALUES_H
#define VALUES_H

#include <functional>

namespace reactive {

template <typename T>
class values {
public:
    using value_type = T;

    explicit values(std::initializer_list<T> values)
        : m_values(values)
    {
    }

    template <typename EmitFunction>
    void on_message(EmitFunction emit)
    {
        m_emit = emit;
        std::for_each(m_values.cbegin(), m_values.cend(), [&] (T value) {
                m_emit(std::move(value));
            });
    }

private:
    std::vector<T> m_values;
    std::function<void(T&&)> m_emit;
};

}

#endif // VALUES_H

