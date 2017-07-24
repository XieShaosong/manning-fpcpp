#ifndef SINK_H
#define SINK_H

#include <functional>

namespace reactive {

namespace detail {
    template < typename Sender
             , typename Function
             , typename MessageType = typename Sender::value_type
             >
    class sink_impl {
    public:
        using value_type = MessageType;

        sink_impl(Sender &&sender, Function function)
            : m_sender(std::move(sender))
            , m_function(function)
        {
            m_sender.on_message(
                [this] (MessageType &&message)
                    {
                        process_message(std::move(message));
                    }
                );
        }

        void process_message(MessageType&& message) const
        {
            std::invoke(m_function, std::move(message));
        }

    private:
        Sender m_sender;
        Function m_function;
    };

    template <typename Function>
    struct sink_helper {
        Function function;
    };

}

template <typename Sender, typename Function>
auto sink(Sender&& sender, Function&& function)
{
    return detail::sink_impl<Sender, Function>(
        std::forward<Sender>(sender),
        std::forward<Function>(function));
}

namespace operators {

    template <typename Function>
    auto sink(Function&& function)
    {
        return detail::sink_helper<Function>{
            std::forward<Function>(function)};
    }

    template <typename Sender, typename Function>
    auto operator|(Sender&& sender,
                   detail::sink_helper<Function> sink)
    {
        return detail::sink_impl<Sender, Function>(
            std::forward<Sender>(sender), sink.function);
    }
}

}

#endif
