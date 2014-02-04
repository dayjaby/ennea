#include "network/connection.hpp"

namespace Ennovia
{

void Connection::async_read(ErrorHandler errorHandler, Handler handler, boost::shared_ptr<Connection> conn)
{

    boost::asio::async_read(socket_, boost::asio::buffer(inbound_header_),
                            boost::bind(&Connection::handle_read_header,
                                        this, boost::asio::placeholders::error,
                                        boost::make_tuple(errorHandler,handler),conn));
}

void Connection::handle_read_header(const boost::system::error_code& e,
                                    boost::tuple<ErrorHandler,Handler> handler, boost::shared_ptr<Connection> conn)

{
    if (e)
    {
        boost::get<0>(handler)(e);
    }
    else
    {
        // Determine the length of the serialized data.
        std::istringstream is(std::string(inbound_header_, header_length));
        std::size_t inbound_data_size = 0;
        MessageHeader msg;
        int x = is.tellg();
        msg.ParseFromIstream(&is);
        is.seekg(header_length);

        inbound_data_size = msg.size();

        // Start an asynchronous call to receive the data.
        inbound_data_.resize(inbound_data_size);
        boost::asio::async_read(socket_, boost::asio::buffer(inbound_data_),
                                boost::bind(&Connection::handle_read_data, this, msg,
                                            boost::asio::placeholders::error, handler, conn));
    }
}

void Connection::handle_read_data(MessageHeader msg, const boost::system::error_code& e,
                                  boost::tuple<ErrorHandler,Handler> handler, boost::shared_ptr<Connection> conn)
{
    if (e)
    {
        boost::get<0>(handler)(e);
    }
    else
    {

        // Extract the data structure from the data just received.
        try
        {
            std::string archive_data(&inbound_data_[0], inbound_data_.size());
            std::istringstream archive_stream(archive_data);
            //boost::archive::binary_iarchive archive(archive_stream);
            boost::get<1>(handler)(msg.id(),archive_stream);
        }
        catch (std::exception& e)
        {
            // Unable to decode data.
            boost::system::error_code error(boost::asio::error::invalid_argument);
            boost::get<0>(handler)(error);
            return;
        }

        // Inform caller that data has been received ok.
        boost::get<0>(handler)(e);
    }
}

void Connection::writeStringStream(int msgid, std::ostringstream& os)
{
    static const char* placeholder = "XXXXXXXXXXXX";
    std::ostringstream header_stream;
    header_stream.write(placeholder,header_length);
    header_stream.seekp(0);

    MessageHeader msg;
    msg.set_size(os.tellp());
    msg.set_id(msgid);
    msg.SerializeToOstream(&header_stream);
    outbound_data_ = header_stream.str();
    outbound_data_ += os.str();
    boost::asio::async_write(socket_, boost::asio::buffer(outbound_data_,outbound_data_.size()),no_handler());
}

}
