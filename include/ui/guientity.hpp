#ifndef ENNOVIA_GUI_ENTITY_HPP
#define ENNOVIA_GUI_ENTITY_HPP

#include "world/entity.hpp"
#include "misc/irrlicht.hpp"

namespace Ennovia {
    class Item;
    class GUIEntity : public Entity {
        GUIEntity();
        virtual ~GUIEntity() {}
        virtual Item* getItem() { return 0; }
    };
}

#endif
