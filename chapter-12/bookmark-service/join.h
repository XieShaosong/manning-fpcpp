#ifndef JOIN_H
#define JOIN_H

#include <functional>
#include <list>

namespace reactive {

namespace detail {
    template < typename Sender
             , typename SourceMessageType = typename Sender::value_type
             , typename MessageType = typename SourceMessageType::value_type
             >
    class join_impl {
    public:
        using value_type = MessageType;

        join_impl(Sender &&sender)
            : m_sender(std::move(sender))
        {
        }

        template <typename EmitFunction>
        void on_message(EmitFunction emit)
        {
            m_emit = emit;
            m_sender.on_message(
                [this] (SourceMessageType &&message)
                    {
                        process_message(std::move(message));
                    }
                );
        }

        void process_message(SourceMessageType &&source)
        {
            m_sources.emplace_back(std::move(source));
            m_sources.back().on_message(m_emit);
        }

    private:
        Sender m_sender;
        std::function<void(MessageType&&)> m_emit;
        std::list<SourceMessageType> m_sources;
    };

    struct join_helper {
    };

}

template <typename Sender>
auto join(Sender&& sender)
{
    return detail::join_impl<Sender>(std::forward<Sender>(sender));
}

namespace operators {

    inline auto join()
    {
        return detail::join_helper();
    }

    template <typename Sender>
    auto operator|(Sender&& sender,
                   detail::join_helper)
    {
        return detail::join_impl<Sender>(std::forward<Sender>(sender));
    }
}

}

#endif
