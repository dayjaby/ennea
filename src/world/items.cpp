#include "world/items.hpp"
#include "world/optionlist.hpp"
#include "world/livingobject.hpp"

namespace Ennovia {

    OptionList* ItemType::getOptionList(LivingObject* o, Item* s)
    {
        return new OptionList();
    }

    OptionList* Item::getOptionList(LivingObject* o) {
        if(type.exists()) {
            return type->getOptionList(o,this);
        } else {
            return new OptionList();
        }
    }

    bool Item::stack(Item& item) {
        if(     type.exists()
           &&   item.type == type
           &&   type->isStackable()) {
            count += item.count;
            item.count = 0;
        }
        return !item.exists();
    }


}
