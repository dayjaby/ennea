#ifndef ENNOVIA_MESSAGE_PROCESSOR_HPP
#define ENNOVIA_MESSAGE_PROCESSOR_HPP

#include <boost/ref.hpp>
#include "core.hpp"
#include "misc/switch.hpp"


namespace Ennovia {


template <class Model>
struct message_processor
{
    typedef typename Model::Connection Connection;
    template <int msgid>
    struct message_table
    {
        static inline int value(std::istream& is, Connection conn)
        {
            std::cout << "Process message "<< msgid << std::endl;
            typedef typename Model::template Interpreter<msgid> Interpreter;
            typedef typename Model::template MessageType<msgid>::type MessageType;
            MessageType msg;
            msg.ParseFromIstream(&is);
            Interpreter::run(msg,conn);
            return 0;
        }
    };

    static void run(int msg, std::istream& stream, Connection conn)
    {
        metalevel::switch_table<0,Model::MESSAGES, message_table>::template run<int>(msg,boost::ref(stream),conn);
    }


};

}

#endif
