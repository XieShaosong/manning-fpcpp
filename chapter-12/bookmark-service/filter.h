#ifndef FILTER_H
#define FILTER_H

#include <functional>

namespace reactive {

namespace detail {

    template < typename Sender
             , typename Predicate
             , typename MessageType = typename Sender::value_type
             >
    class filter_impl {
    public:
        using value_type = MessageType;

        filter_impl(Sender &&sender, Predicate predicate)
            : m_sender(std::move(sender))
            , m_predicate(predicate)
        {
        }

        template <typename EmitFunction>
        void on_message(EmitFunction emit)
        {
            m_emit = emit;
            m_sender.on_message(
                [this] (MessageType &&message)
                    {
                        process_message(std::move(message));
                    }
                );
        }

        void process_message(MessageType &&message) const
        {
            if (m_emit && std::invoke(m_predicate, message)) {
                m_emit(std::move(message));
            }
        }

    private:
        Sender m_sender;
        Predicate m_predicate;
        std::function<void(MessageType&&)> m_emit;
    };

    template <typename Predicate>
    struct filter_helper {
        Predicate predicate;
    };

}

template <typename Sender, typename Predicate>
auto filter(Sender&& sender, Predicate&& predicate)
{
    return detail::filter_impl<Sender, Predicate>(
        std::forward<Sender>(sender),
        std::forward<Predicate>(predicate));
}

namespace operators {

    template <typename Predicate>
    auto filter(Predicate&& predicate)
    {
        return detail::filter_helper<Predicate>{
            std::forward<Predicate>(predicate)};
    }

    template <typename Sender, typename Predicate>
    auto operator| (Sender&& sender, detail::filter_helper<Predicate> filter)
    {
        return detail::filter_impl<Sender, Predicate>(
            std::forward<Sender>(sender), filter.predicate);
    }
}

}

#endif
