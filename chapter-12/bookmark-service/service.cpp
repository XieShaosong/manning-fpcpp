
#include "service.h"

service::service(boost::asio::io_service& service,
                 unsigned short port)
    : m_acceptor(service, tcp::endpoint(tcp::v4(), port))
    , m_socket(service)
{
}

void service::do_accept()
{
    m_acceptor.async_accept(
        m_socket, [this] (const boost::system::error_code& error) {
            if (!error) {
                // Creating a new session and start listing for
                // client messages
                make_shared_session(
                        std::move(m_socket),
                        m_emit
                    )->start();

            } else {
                // If there was a connection error,
                // just write it out
                std::cerr << error.message() << std::endl;

            }

            // Listening to another client
            do_accept();
        });
}

