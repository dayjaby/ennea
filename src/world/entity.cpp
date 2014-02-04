#include "world/entity.hpp"
#include "world/optionlist.hpp"

namespace Ennovia {

OptionList* Entity::getOptionList(LivingObject* o) {
    return new OptionList();
}

}
