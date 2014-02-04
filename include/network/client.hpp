#ifndef ENNOVIA_NETWORK_CLIENT_HPP
#define ENNOVIA_NETWORK_CLIENT_HPP


#include "network/connection.hpp"

namespace Ennovia
{

class Client
{
public:

    /// Constructor starts the asynchronous connect operation.
    Client(boost::asio::io_service& io_service,
           std::string host, std::string service);

    template <int msgid, typename Type>
    void write(Type& t)
    {
        std::cout << "Write message " << msgid << std::endl;
        connection->write<msgid>(t);
    }

    boost::shared_ptr<Connection> connection;
private:
    /// Handle completion of a connect operation.
    void handle_connect(const boost::system::error_code& e);

    /// Start to read a message
    void start_read();

    /// Handle a finished read operation
    void finished_read(const boost::system::error_code& e);

    /// Interpret a read message
    void interpret(int msgid, std::istream& is);


};
}

#endif
