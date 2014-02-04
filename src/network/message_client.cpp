#include "network/network.hpp"

namespace Ennovia {
/*
INTERPRET(ClientModel,SEND_ENTITIES,MsgSendEntities)
{
    std::cout << "send entities interpreter" << std::endl;

   std::vector<reg<
    for(int i=0;i<data.entities_size();i++) {
        PEntity pEntity = data.entities(i);
        // ...
    }
}


INTERPRET(ClientModel,DEFAULT,NoData) {
    std::cout << "default message interpreter" << std::endl;
}
INTERPRET(ClientModel,SEND_OPTION_LIST,PEntity) {
    std::cout << "send option list interpreter" << std::endl;
}
INTERPRET(ClientModel,SEND_MESSAGE,MsgSendMessage) {
    MsgSendMessage response;
    std::cout << "[MSG from Server]: " << data.text() << std::endl;
    std::cout << "Your response: ";
    std::cin >> *response.mutable_text();
    connection->write<ServerModel::SEND_MESSAGE>(response);
}
*/

}
