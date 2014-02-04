#ifndef ENNOVIA_PLAYER_HPP
#define ENNOVIA_PLAYER_HPP

#include "world/livingobject.hpp"
#include "world/items.hpp"

namespace Ennovia
{
class Player : public LivingObject
{
public:
    Player(const Position& position, const std::string& name_);
    Item* currentItem;
    bool hasCurrentItem();

    virtual OptionList* getOptionList(LivingObject* lo);
    virtual void onGainExp(std::string skillname, int amount);
    virtual void onLevelup(std::string skillname, int lvl);


 //   virtual bool isPlayer() { return true; }
};
}

#endif
