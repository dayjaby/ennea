#include "world/object.hpp"

namespace Ennovia {
    /// Object

    Object::Object(const Position& position, const std::string& name_) : Locatable(position), name(name_) {
    }

    Object::~Object() {
    }

    OptionList* Object::getOptionList(Player* p) {
        OptionList* list = new OptionList();
        std::cout << "getting optionlist" << std::endl;
        //list.add(new Examine<Object,Player>(this,p));
        return list;
    }

    std::string Object::getName() {
        return name;
    }

    std::string Object::onExamine(Player* o) {
        std::cout << "examining" << std::endl;
        return name;
    }
}
