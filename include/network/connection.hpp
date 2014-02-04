#ifndef ENNOVIA_CONNECTION_HPP
#define ENNOVIA_CONNECTION_HPP

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

#include "misc/registry.hpp"
#include "network/proto/msg.pb.h"

namespace Ennovia
{
struct no_handler
{
    void operator()() {}
    template <class T1>
    void operator()(T1 t1) {}
    template <class T1, class T2>
    void operator()(T1 t1, T2 t2) {}
};

class Player;
class Connection
{
public:
    typedef boost::function<void(const boost::system::error_code&)> ErrorHandler;
    typedef boost::function<void(int,std::istream&)> Handler;

    Connection(boost::asio::io_service& io_service)
        : socket_(io_service)
    {
    }


    boost::asio::ip::tcp::socket& socket()
    {
        return socket_;
    }

    void disconnect() {
        boost::system::error_code ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    }

    template <int msgid,class Type>
    void write(Type& t)

    {
        std::ostringstream archive_stream;
        t.SerializeToOstream(&archive_stream);
        writeStringStream(msgid,archive_stream);
    }

    template <int msgid>
    void write()

    {
        std::ostringstream archive_stream;
        writeStringStream(msgid,archive_stream);
    }

    void async_read(ErrorHandler errorHandler, Handler handler, boost::shared_ptr<Connection> conn);

    void handle_read_header(const boost::system::error_code& e,
                            boost::tuple<ErrorHandler,Handler> handler, boost::shared_ptr<Connection> conn);


    void handle_read_data(MessageHeader msg, const boost::system::error_code& e,
                          boost::tuple<ErrorHandler,Handler> handler, boost::shared_ptr<Connection> conn);
    reg<Player> player;

private:

    void writeStringStream(int msgid, std::ostringstream&  ostream);


    /// The underlying socket.
    boost::asio::ip::tcp::socket socket_;

    /// The size of a fixed length header.
    enum { header_length = 8 };

    /// Holds an outbound header.
    std::string outbound_header_;

    /// Holds the outbound data.
    std::string outbound_data_;

    /// Holds an inbound header.
    char inbound_header_[header_length];

    /// Holds the inbound data.
    std::vector<char> inbound_data_;
};



}
#endif
