#ifndef ENNOVIA_TILEPROXY_HPP
#define ENNOVIA_TILEPROXY_HPP

#include "world/tile.hpp"

namespace Ennovia {
class TileProxy : public Tile
{
public:
    virtual OptionList* getOptionList(Player* o);
};
}
#endif
