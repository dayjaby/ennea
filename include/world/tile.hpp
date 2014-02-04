#ifndef ENNOVIA_TILE_HPP
#define ENNOVIA_TILE_HPP

#include <set>

#include "world/locatable.hpp"
namespace Ennovia
{

class Tile : public Locatable
{
public:
    typedef std::set<Locatable*> Locatables;
    Tile();
    Tile(const Position& position, const std::string& name);
    Tile(const Tile& tile);
    virtual ~Tile();
    virtual OptionList* getOptionList(LivingObject* o);
    Locatables& getEntitiesOnTile();
    void enter(Locatable* loc);
    void leave(Locatable* loc);

    bool canMove[4];
protected:
    virtual void onEnter(Locatable* loc) {}
    virtual void onLeave(Locatable* loc) {}
private:
    Locatables locatables;
};


}
#endif // ENNOVIA_TILE_HPP
