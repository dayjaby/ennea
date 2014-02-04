#include "world/tile.hpp"
#include "world/map.hpp"
#include "world/optionlist.hpp"

namespace Ennovia
{

Tile::Tile() : Locatable(Position(0,0,0),std::string("Nothing")) {
}

Tile::Tile(const Position& position, const std::string& name) : Locatable(position,name) {}

Tile::Tile(const Tile& tile) : Locatable(tile) {}

Tile::~Tile() {
}

OptionList* Tile::getOptionList(LivingObject* o)
{
    #ifdef ENNOVIA_SERVER
    OptionList* list = new OptionList();
    //if(canMove[0] || canMove[1] || canMove[2] || canMove[3])
//        list->add(new WalkTo(getPosition()));
//    list.add(new Examine<Tile,TileType>(this,type));
    return list;
    #else
    return Locatable::getOptionList(o);
    #endif
}

Tile::Locatables& Tile::getEntitiesOnTile()
{
    return locatables;
}

void Tile::enter(Locatable* locatable)
{
    locatables.insert(locatable);
    onEnter(locatable);
}
void Tile::leave(Locatable* locatable)
{
    locatables.erase(locatable);
    onLeave(locatable);
}



} // namespace Ennovia
