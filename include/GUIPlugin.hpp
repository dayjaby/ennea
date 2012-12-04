#ifndef ENNOVIA_GUIPLUGIN_HPP
#define ENNOVIA_GUIPLUGIN_HPP

namespace Ennovia {
    template <class Adapter>
    class GUIPlugin<Adapter> {
        virtual void create(Adapter& adapter) = 0;
        virtual void update() = 0;
        virtual void close() = 0;
    };
}

#endif
