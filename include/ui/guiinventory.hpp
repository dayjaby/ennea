#ifndef ENNOVIA_WINDOWS_INVENTORY_HPP
#define ENNOVIA_WINDOWS_INVENTORY_HPP

#include "ui/windows/window.hpp"
#include "ui/guientity.hpp"

namespace Ennovia {

    class Inventory;
    class GUIInventory : public irr::gui::IGUIElement {
    public:
        GUIInventory(irr::gui::IGUIElement* parent,Inventory& inv);
        Inventory& getInventory() { return inventory; }
        void update();
    private:
        Inventory& inventory;
    };
}

#endif
