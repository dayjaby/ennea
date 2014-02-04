#ifndef ENNOVIA_MESSAGE_DECL_HPP
#define ENNOVIA_MESSAGE_DECL_HPP

namespace Ennovia {


struct ServerModel
{
    enum { CHOOSE_OPTION = 0, GET_OPTION_LIST = 1, GET_ENTITIES = 2, MESSAGES};

    static const int DEFAULT = MESSAGES;

    typedef boost::shared_ptr<Connection_<ServerModel> > Connection;

    template <int msgid> struct MessageType
    {
        typedef NoData type;
    };

    template <int msgid> struct Interpreter
    {
        static void run(typename MessageType<msgid>::type&, Connection);
    };
};

struct ClientModel
{
    enum { SEND_OPTION_LIST = 0, SEND_ENTITIES = 1,  MESSAGES};

    static const int DEFAULT = MESSAGES;

    typedef boost::shared_ptr<Connection_<ClientModel> > Connection;


    template <int msgid> struct MessageType
    {
        typedef NoData type;
    };

    template <int msgid> struct Interpreter
    {
        static void run(typename MessageType<msgid>::type&, Connection);
    };

};


}

#endif
