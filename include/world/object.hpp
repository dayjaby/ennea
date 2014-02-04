#ifndef ENNOVIA_OBJECT_HPP
#define ENNOVIA_OBJECT_HPP

#include "world/locatable.hpp"

namespace Ennovia {


    class Object : public Locatable
    {
    public:
        Object(const Position& position, const std::string& name_);
        virtual ~Object();


    };
}

#endif
