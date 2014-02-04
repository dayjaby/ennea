#ifndef ENNOVIA_FACTORY_HPP
#define ENNOVIA_FACTORY_HPP

namespace Ennovia {
class Factory {
public:
    void register_lua(lua_State* L) {
        module(L)
        [
            def("createItem",boost::bind(Factory::createItemX,this)),
            def("registerItem",boost::bind(Factory::registerItem,this))
        ];
    }

    Item* createItemX(const std::string& id, int qty, const luabind::object& data) {
        return call_function<Item*>(globals(L)["factories"]["items"][id], qty, data) [ adopt(result) ];
    }

    Item* createItem(const std::string& id, int qty) {
        return createItem(id,qty,luabind::nil);
    }
private:
    void registerItem(const std::string& id, const luabind::object& constructor);
};

/** In Lua:

class Bucket ('Item')
...

registerItem(OFFICIAL_PREFIX + 'Bucket', Bucket)

In C++:

Item* createItem(id, quantity, void* data) {

}


**/
}

#endif
