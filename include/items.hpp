#ifndef ENNOVIA_ITEMS_HPP
#define ENNOVIA_ITEMS_HPP

#include <irrlicht/irrlicht.h>
#include <algorithm>
#include <list>
#include <iostream>
#include "entity.hpp"
#include "actions.hpp"

namespace Ennovia {


    class Item;

    class ItemType
    {
    public:
        ItemType(std::string name_, std::string path_) : name(name_), path(path_) {}
        virtual bool isStackable() { return false; }
        virtual std::string onExamine(Item* s) { return "The default ItemType"; }
        virtual std::string getName() { return name; }
        std::string getPath() { return path; }
        virtual OptionList getOptionList(Player* o, Item* s) {
            return OptionList();
        }
    private:
        std::string name;
        std::string path;
    };

    class Item
    {
    public:
        Item() : type(0), amount(0), quality(0) {}
        Item(ItemType* item_) : type(item_), amount(1), quality(0) {}
        Item(ItemType* item_, int amount_) : type(item_), amount(amount_), quality(0) {}
        Item(ItemType* item_, int amount_, int quality_)
        : type(item_), amount(amount_), quality(quality_) {}

        static Item Empty() {
            return Item(0,0);
        }
        void swap(Item& s) {
            std::swap(amount,s.amount);
            std::swap(quality,s.quality);
            std::swap(type,s.type);
        }
        ItemType* type;
        int amount;
        int quality;

        void changeItemType(ItemType* item_) {
            type = item_;
        }


        OptionList getOptionList(Player* o) {
            if(!type) return OptionList();
            OptionList list = type->getOptionList(o,this);
            list.add(new Examine<Item,ItemType>(this,type));
            return list;
        }
    };

}
#endif
