#include "network/client.hpp"
#include "network/message.hpp"
#include "world/player.hpp"
#include "world/optionlist.hpp"
#include "mayor.hpp"

namespace Ennovia
{

Client::Client(boost::asio::io_service& io_service,
               std::string host, std::string service)
    : connection(new Connection(io_service))
{
    // Resolve the host name into an IP address.
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(host, service);
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator =
        resolver.resolve(query);
    std::cout << "Establish Connection..." << std::endl;

    // Start an asynchronous connect operation.
    boost::asio::async_connect(connection->socket(), endpoint_iterator,
                               boost::bind(&Client::handle_connect, this,
                                           boost::asio::placeholders::error));
}


/// Handle completion of a connect operation.
void Client::handle_connect(const boost::system::error_code& e)
{
    if (!e)
    {
        start_read();
    }
    else
    {
        std::cerr << e.message() << std::endl;
    }
}

void Client::start_read()
{
    connection->async_read(boost::bind(&Client::finished_read, this,
                                 boost::asio::placeholders::error),
                     boost::bind(&Client::interpret, this,_1,_2),
                     connection);
}

void Client::finished_read(const boost::system::error_code& e)
{
    if(e)
    {
        std::cerr << e.message() << std::endl;
    }
    start_read();
}

void Client::interpret(int msgid, std::istream& is)
{
    Mayor& mayor = Mayor::get();
    mayor.log << "Received message: " << msgid << std::endl;
    std::cout << "Received message: " << msgid << std::endl;
    switch(msgid)
    {
    case SEND_MESSAGE:
    {
        ServerMessage in;
        in.ParseFromIstream(&is);
        std::cout << "[MSG from Server]: " << in.text() << std::endl;
        mayor.log << "[MSG from Server]: " << in.text() << std::endl;
    }
    break;
    case SEND_LOCATABLE_OPTIONLIST:
    {
        SendLocatableOptionList in;
        in.ParseFromIstream(&is);
        OptionList* optionList = new OptionList();
        for(int i=0;i<in.options_size();i++) {
            const OptionRef& option = in.options(i);
            optionList->add(new OptionProxy(option.description(),option.optionlist(),option.index()));
        }
        mayor.receiveLocatableOptionList(in.id(),optionList);
    }
    break;
    case MOVE_TO:
    {
        MoveTo in;
        in.ParseFromIstream(&is);
        mayor.moveTo(in.id(),in.x(),in.y());
    }
    break;
    case REQUEST_CREDENTIALS:
    {
        mayor.onRequestCredentials();
    }
    break;
    case LOGIN_USERNAME_INVALID:
    {
        mayor.onLoginUsernameInvalid();
    }
    break;
    case LOGIN_PASSWORD_INVALID:
    {
        mayor.onLoginPasswordInvalid();
    }
    break;
    case LOGIN_VALID:
    {
        mayor.onLoginValid();
    }
    break;
    case SEND_AES_KEY:
    {

    }
    break;
    case YOU_ARE:
    {
        YouAre id;
        id.ParseFromIstream(&is);
        mayor.youAre(id.id());
        mayor.log << "You are " << id.id();
    }
    break;
    case LOCATABLE_POSITION:
    {
        LocatablePosition pos;
        pos.ParseFromIstream(&is);
        mayor.setLocatablePosition(pos.id(),pos.map(),pos.x(),pos.y());
        mayor.log << "Lo" << pos.id() << "@" << pos.map() << "," << pos.x() << "," << pos.y() << std::endl;
    }
    break;
    case INTRODUCE_LOCATABLE:
    {
        IntroduceLocatable intro;
        intro.ParseFromIstream(&is);
        mayor.introduceLocatable(intro.id(),intro.type(),intro.name());
        mayor.log << "Introduction " << intro.id() << "," << intro.type() << "," << intro.name() << std::endl;
        if(intro.has_model()) {
            mayor.log << "Set Model " << intro.model();
            mayor.setLocatableModel(intro.id(),intro.model(),intro.has_texture() ? intro.texture() : std::string(""));
        }
    }
    break;
    case SEND_MAP:
    {
        SendMapData mapData;
        mapData.ParseFromIstream(&is);
        mayor.sendMapData(mapData.id(),mapData.path(),mapData.heightmap(),mapData.width(),mapData.height());
        mayor.log << "Map " << mapData.id() << "=" << mapData.path() << "," << mapData.heightmap() << "," << mapData.width() << "*" << mapData.height();
    }
    break;
    default:
        break;
    }

    mayor.log << "|" << std::endl;


}
}

