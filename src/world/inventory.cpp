#include "world/inventory.hpp"
#include "world/locatable.hpp"
#include "misc/registry.hpp"
namespace Ennovia {

    Inventory::Inventory(Locatable* locatable)
        : owner(locatable),width_(4),height_(6) {
        items.resize(width_*height_);
    }
    int Inventory::width() {
        return width_;
    }
    int Inventory::height() {
        return height_;
    }
    bool Inventory::access(reg<Locatable> other) {
        /// TODO: Loot share/special accessed inventories...
        return true;
    }
    void Inventory::resize(int w,int h) {
        items.resize(w*h);
    }
    Item& Inventory::item(int x,int y) {
        return items[x*height_+y];
    }
    bool Inventory::place(Item& i) {
        if(!i.exists()) return true;
        int w = width();
        int h = height();
        for(int y=0;y<h;y++) {
            for(int x=0;x<w;x++) {
                if(i.exists() && item(x,y).stack(i)) return true;
            }
        }
        return false;
    }
    bool Inventory::place(int x,int y,Item& i) {
        return item(x,y).stack(i);
    }
    Item Inventory::take(int x,int y) {
        Item i = item(x,y);
        item(x,y).count = 0;
        return i;
    }
    bool Inventory::takeAll(reg<Locatable> other) {
        bool result = true;
        if(access(other)) {
            int w = width();
            int h = height();
            for(int y=0;y<h;y++) {
                for(int x=0;x<w;x++) {
                    if(!other->inventory().place(item(x,y)))
                        result = false;
                }
            }
        }
        return result;
    }

}
