#include "world/tileproxy.hpp"

namespace Ennovia {
OptionList* TileProxy::getOptionList(Player* o) {
    return Mayor::get().getOptionList(*this);
}

}
