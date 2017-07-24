#ifndef SERVICE_H
#define SERVICE_H

// Standard library
#include <iostream>
#include <functional>

// Boost ASIO
#include <boost/asio.hpp>

// Not a good idea to add a using declaration in a header,
// but we can live with it for this small example
using boost::asio::ip::tcp;


namespace detail {

/**
 * A structure to contain a value of any type,
 * and a contextual information - the socket
 * to the client.
 *
 * It is dangerous to pass around a raw pointer
 * to a socket. Usually, we would pass a client
 * identifier instead, and have the central
 * server component keep track of all clients.
 */
template <typename MessageType>
struct with_client {
    MessageType value;
    tcp::socket *socket;

    void reply(const std::string& message) const
    {
        boost::asio::async_write(
                *socket,
                boost::asio::buffer(message, message.length()),
                                 [](auto, auto) {});
    }
};

} // namespace detail

/**
 * Function that constructs an instance of with_client given a
 * value and a socket
 */
template <typename MessageType>
auto make_with_client(MessageType&& value, tcp::socket* socket)
{
    return ::detail::with_client<MessageType>{
        std::forward<MessageType>(value), socket};
}

/**
 * Lift any function from T1 to T2 to a function
 * from with_client<T1> to with_client<T2>
 */
template <typename F>
auto lift_with_client(F&& function)
{
    return [function = std::forward<F>(function)] (auto &&ws) {
        return make_with_client(std::invoke(function, ws.value), ws.socket);
    };
}

/**
 * Converts a function from T1 to T2 to a
 * function from with_client<T1> to T2.
 * This is not a given thing for all monads,
 * but we need it to be able to use the filter
 * transformation on streams that emit values
 * with socket pointer
 */
template <typename F>
auto apply_with_client(F&& function)
{
    return [function = std::forward<F>(function)] (auto &&ws) {
        return std::invoke(function, std::forward<decltype(ws)>(ws).value);
    };
}



/**
 * Session handling class.
 *
 * It reads the data sent by the client line by line,
 * and sends each line as a separate message.
 */
template <typename EmitFunction>
class session: public std::enable_shared_from_this<session<EmitFunction>> {
public:
    session(tcp::socket&& socket, EmitFunction emit)
        : m_socket(std::move(socket))
        , m_emit(emit)
    {
    }

    void start()
    {
        do_read();
    }

private:
    using shared_session = std::enable_shared_from_this<session<EmitFunction>>;

    void do_read()
    {
        // Getting a shared pointer to this instance
        // to capture it in the lambda
        auto self = shared_session::shared_from_this();
        boost::asio::async_read_until(
            m_socket, m_data, '\n',
            [this, self](const boost::system::error_code& error,
                         std::size_t size) {
                if (!error) {
                    // Reading a line from the client and
                    // passing it to whoever listens to us
                    std::istream is(&m_data);
                    std::string line;
                    std::getline(is, line);
                    m_emit(make_with_client(std::move(line), &m_socket));

                    // Scheduling the next line to be read
                    do_read();
                }
            });
    }

    tcp::socket m_socket;
    boost::asio::streambuf m_data;
    EmitFunction m_emit;
};

/**
 * Constructs a shared pointer to the session specified
 * by the socket and sets the function which the session will
 * used for sending the messages
 */
template <typename Socket, typename EmitFunction>
auto make_shared_session(Socket&& socket, EmitFunction&& emit)
{
    return std::make_shared<session<EmitFunction>>(
            std::forward<Socket>(socket),
            std::forward<EmitFunction>(emit));
}

/**
 * The service class handles client connections
 * and emits the messages sent by the clients
 */
class service {
public:
    using value_type = ::detail::with_client<std::string>;

    explicit service(boost::asio::io_service& service,
                     unsigned short port = 42042);

    service(const service &other) = delete;
    service(service &&other) = default;

    template <typename EmitFunction>
    void on_message(EmitFunction emit)
    {
        m_emit = emit;
        do_accept();
    }

private:
    void do_accept();

    tcp::acceptor m_acceptor;
    tcp::socket m_socket;
    std::function<void(::detail::with_client<std::string>&&)> m_emit;

    friend std::ostream& operator<< (std::ostream& out, const service& service)
    {
        return out << "service object";
    }
};

#endif
