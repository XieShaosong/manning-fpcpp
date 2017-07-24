#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <functional>

namespace reactive {

namespace detail {
    template < typename Sender
             , typename Transformation
             , typename SourceMessageType = typename Sender::value_type
             , typename MessageType = decltype(std::declval<Transformation>()(std::declval<SourceMessageType>()))
             >
    class transform_impl {
    public:
        using value_type = MessageType;

        transform_impl(Sender &&sender, Transformation transformation)
            : m_sender(std::move(sender))
            , m_transformation(transformation)
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

        void process_message(SourceMessageType&& message) const
        {
            m_emit(std::invoke(m_transformation, std::move(message)));
        }

    private:
        Sender m_sender;
        Transformation m_transformation;
        std::function<void(MessageType&&)> m_emit;
    };

    template <typename Transformation>
    struct transform_helper {
        Transformation function;
    };

}

template <typename Sender, typename Transformation>
auto transform(Sender&& sender, Transformation&& transformation)
{
    return detail::transform_impl<Sender, Transformation>(
        std::forward<Sender>(sender),
        std::forward<Transformation>(transformation));
}

namespace operators {

    template <typename Transformation>
    auto transform(Transformation&& transformation)
    {
        return detail::transform_helper<Transformation>{
            std::forward<Transformation>(transformation)};
    }

    template <typename Sender, typename Transformation>
    auto operator|(Sender&& sender,
                   detail::transform_helper<Transformation> transformation)
    {
        return detail::transform_impl<Sender, Transformation>(
            std::forward<Sender>(sender), transformation.function);
    }
}

}

#endif
