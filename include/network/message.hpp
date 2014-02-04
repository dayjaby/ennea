#ifndef ENNOVIA_MESSAGE_HPP
#define ENNOVIA_MESSAGE_HPP

#include "network/proto/client.pb.h"
#include "network/proto/server.pb.h"

namespace Ennovia
{
    /** Server -> Client Messages **/
    enum {
        SEND_MESSAGE = 3,
        /** Option Lists **/
        SEND_TILE_OPTIONLIST = 100,
        SEND_LOCATABLE_OPTIONLIST = 101,
        MOVE_TO = 102,
        /** Login / security messages **/
        REQUEST_CREDENTIALS = 200,
        LOGIN_USERNAME_INVALID = 201,
        LOGIN_PASSWORD_INVALID = 202,
        LOGIN_VALID = 203,
        SEND_RSA_KEY = 204,
        /** Entities **/
        YOU_ARE = 300,
        LOCATABLE_POSITION = 301,
        INTRODUCE_LOCATABLE = 302,
        /** Maps **/
        SEND_MAP = 400
    };

    /** Client -> Server Messages **/
    enum {
        MESSAGE = 2,
        /** Option Lists **/
        GET_TILE_OPTIONLIST = 100,
        GET_LOCATABLE_OPTIONLIST = 101,

        CHOOSE_OPTION = 103,
        GET_LOCATABLE_POSITION = 104,
        GET_LOCATABLE_INTRODUCTION = 105,
        WALK_TO = 106,

        /** Login / security messages **/
        CREDENTIALS = 200,
        SEND_AES_KEY = 204,
        /** Maps **/
        REQUEST_MAP = 400
    };


}

#endif
