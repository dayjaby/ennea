#ifndef ENNOVIA_PLUGINS_CHEST_HPP
#define ENNOVIA_PLUGINS_CHEST_HPP

#include "plugins/GUIPlugin.hpp"

namespace Ennovia {
    class GUIChest : public GUIPlugin {
    public:
        GUIChest(Chest& chest_)
        : GUIPlugin(guienv->addWindow(core::rect<s32>(200,200,chest_.getWidth()*48+204,chest_.getHeight()*48+222),
                                      false,stringw(chest_.getName().c_str()).c_str())), chest(chest_)
        {
        }
        virtual void open() {
            if(&chest == openChest) return;
            if(openChest) {
                close();
            }
            openChestElement = (IGUIWindow*) getElement();
            openChestElement->getMinimizeButton()->setVisible(true);
            openChest = &chest;
            update();
        }

        virtual void update(Player& player) {
            if(!openChest) return;
            core::list<IGUIElement*> children = openChestElement->getChildren();
            for(core::list<IGUIElement*>::Iterator i = children.begin(); i!=children.end();++i) {
                if((*i)->getID()==0) (*i)->remove();
            }
            for(int x=0;x<openChest->getWidth();x++) {
                for(int y=0;y<openChest->getHeight();y++) {
                    if(openChest->getItemAt(x,y).type) {
                        IGUIElement* dataElement =
                            new GUIDataElement<boost::tuple<Chest*,int,int> >(boost::tuple<Chest*,int,int>(openChest,x,y),core::rect<s32>(x*48+2,y*48+20,x*48+50,y*48+68),guienv,openChestElement);
                        IGUIImage* image = guienv->addImage(getTexture(openChest->getItemAt(x,y).type->getPath()),core::position2d<s32>(0,0),true,dataElement);
                        core::stringw word(openChest->getItemAt(x,y).amount);
                        int x2 = 48-fonts[2]->getWidthFromWord(word);
                        IGUIStaticText* text = guienv->addStaticText(word.c_str(),core::rect<s32>(x2,32,48,48),false,false,dataElement);
                        text->enableOverrideColor(true);
                        text->setOverrideColor(irr::video::SColor(255,255,0,0));
                        text->setOverrideFont(fonts[2]);
                    }
                }
            }

            if(player.getPosition().getSquaredDistance(chest.getPosition()) > chest.getAccessDistance() * chest.getAccessDistance()) {
                close();
            }
        }

        virtual void close() {
            if(openChest == &chest) {
                openChest = 0;
                openChestElement->remove();
            }
        }
    protected:
        static Chest* openChest;
        static IGUIWindow* openChestElement;
        Chest& chest;
    };
}

#endif

