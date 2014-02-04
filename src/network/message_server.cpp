#include "network/network.hpp"
#include "network/proxies.hpp"

namespace Ennovia {
/*
INTERPRET(ServerModel,DEFAULT,NoData) {}
INTERPRET(ServerModel,GET_ENTITIES,NoData) {}

INTERPRET(ServerModel,CHOOSE_OPTION,POptionRef)
{
    //getOption(data).onChoose(connection->player);
}
INTERPRET(ServerModel,GET_OPTION_LIST,PEntity)
{
    // client requests the option list of an entity
    //Entity* entity = getEntity(data);
    //*data.mutable_optionlist() ;
    //getPOptionList(entity->getOptionList(connection->player));
    //entity->getOptionList(connection->player);
    //connection->write<ClientModel::SEND_OPTION_LIST>(data);
}

INTERPRET(ServerModel,SEND_MESSAGE,MsgSendMessage)
{
    MsgSendMessage response;
    std::cout << "[MSG from Client]: " << data.text() << std::endl;
    std::cout << "Your response: ";
    std::cin >> *response.mutable_text();
    connection->write<ClientModel::SEND_MESSAGE>(response);
}
*/

}
