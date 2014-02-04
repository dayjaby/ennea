#ifndef ENNOVIA_INVENTORY_HPP
#define ENNOVIA_INVENTORY_HPP

#include <boost/multi_array.hpp>
#include "world/items.hpp"
#include "misc/registry_forw.hpp"

namespace Ennovia {
    class Locatable;

    class Inventory {
    public:
        typedef std::vector<Item> Items;
        Inventory(Locatable* locatable);
        virtual ~Inventory() {}
        int width();
        int height();
        virtual bool access(reg<Locatable> other);
        void resize(int w,int h);
        Item& item(int x,int y);
        bool place(Item& i);
        bool place(int x,int y,Item& i);
        Item take(int x,int y);
        bool takeAll(reg<Locatable> other);
        Locatable* owner;
    private:
        int width_,height_;
        Items items;
    };

}

#endif
