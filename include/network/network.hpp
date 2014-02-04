#ifndef ENNOVIA_NETWORK_HPP
#define ENNOVIA_NETWORK_HPP

#include "network/connection.hpp"
#include "network/message.hpp"
#include "network/client.hpp"

namespace Ennovia {
    typedef boost::shared_ptr<Connection> ServerConnection;
    typedef boost::shared_ptr<Connection> ClientConnection;


}
#endif
