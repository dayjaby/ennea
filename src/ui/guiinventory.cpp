#include "ui/graphics.hpp"
#include "ui/guiinventory.hpp"
#include "mayor.hpp"
#include "world/inventory.hpp"

namespace Ennovia {

    GUIInventory::GUIInventory(irr::gui::IGUIElement* parent, Inventory& inv)
    : IGUIElement(irr::gui::EGUIET_ELEMENT,RENDER.getGUIEnvironment(),parent,0,irr::core::rect<s32>(0,0,inv.width()*48,inv.height()*48)), inventory(inv) {
        update();
    }

    void GUIInventory::update() {
        irr::video::ITexture* empty = RENDER.getTexture("media/inv_space.png");
        for(int x=0;x<inventory.width();x++) {
            for(int y=0;y<inventory.height();y++) {
                GUIDataElement<Item>* elem =
                    new GUIDataElement<Item>(&inventory.item(x,y), irr::core::rect<s32>(x*48,y*48,(x+1)*48,(y+1)*48), RENDER.getGUIEnvironment(), this);
                RENDER.getGUIEnvironment()->addImage(empty,irr::core::position2d<s32>(0,0),true,elem,1);
            }
        }
    }
}
