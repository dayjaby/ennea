#ifndef ENNOVIA_LUA_MAP_HPP
#define ENNOVIA_LUA_MAP_HPP

#include <luabind/luabind.hpp>
#include "world/map.hpp"

namespace Ennovia {
    namespace Lua {
        using namespace luabind;
        scope registerMap() {
            return class_<Map>("Map")
                .def(constructor<int,const std::string&,const std::string&,int,int>())
                .def("getHeight",&Map::getHeight);
        }
    }
}

#endif
