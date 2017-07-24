// vim: set undofile:
// Program: 11-make_curried

#include <tuple>
#include <string>
#include <iostream>
#include <functional>
#include <type_traits>
#include <utility>

template <typename Function, typename ...CapturedArgs>
class curried {
public:
    curried(Function function, CapturedArgs... args)
        : m_function(function)
        , m_captured(std::make_tuple(args...))
    {
    }

    curried(Function function, std::tuple<CapturedArgs...> args)
        : m_function(function)
        , m_captured(args)
    {
    }

    template <typename ...NewArgs>
    auto operator()(NewArgs&& ...args) const
    {
        auto new_args = std::make_tuple(std::forward<NewArgs>(args)...);
        auto all_args = std::tuple_cat(m_captured, std::move(new_args));

        if constexpr(std::is_invocable_v<Function, CapturedArgs...,
                                         NewArgs...>) {
            return std::apply(m_function, all_args);

        } else {
            return curried<Function, CapturedArgs..., NewArgs...>(
                m_function, all_args);
        }
    }

private:
    Function m_function;
    std::tuple<CapturedArgs...> m_captured;
};


// Needed for pre-C++17 compilers
template <typename Function>
curried<Function> make_curried(Function &&f)
{
    return curried<Function>(std::forward<Function>(f));
}



class callable_test {
public:
    template <typename T1, typename T2, typename T3>
    auto operator() (T1 x, T2 y, T3 z) const
    {
        return x + y + z;
    }

    template <typename T1, typename T2>
    auto operator() (T1 x, T2 y) const
    {
        return x + y;
    }

};


int main(int argc, char *argv[])
{
    auto less_curried = curried(std::less<>());

    std::cout << less_curried(42, 1) << std::endl;

    auto greater_than_42 = less_curried(42);

    std::cout << greater_than_42(1.0) << std::endl;
    std::cout << greater_than_42(100.0) << std::endl;



    callable_test ct;

    auto ct_curried = curried(ct);

    std::cout << ct_curried(1)(2,3) << std::endl;

    auto ct_curried_one = curried(ct, 1);

    std::cout << ct_curried_one(2,3) << std::endl;

    return 0;
}


