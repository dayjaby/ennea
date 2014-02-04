#include "world/locatable.hpp"
#include "world/tile.hpp"
#include "world/map.hpp"
#include "world/optionlist.hpp"
#include "misc/irrlicht.hpp"
#include "world/library.hpp"
#include "mayor.hpp"

namespace Ennovia
{


irr::ITimer*& timer()
{
    static irr::ITimer* myTimer = 0;
    return myTimer;
}

Locatable::Locatable(const Position& pos, const std::string& name_, const std::string& model, const std::string& texture)
    : actionEnd(0), name(name_), model_(model), texture_(texture), inventory_(this)
{
    //setPosition(pos);
}

Locatable::Locatable(const Position& pos, const std::string& name_) : actionEnd(0), name(name_), inventory_(this)
{
    //setPosition(pos);
}

Locatable::~Locatable()
{
    //if(position_.map)
    //    position_.map->getTile(position_.x,position_.y).leave(this);
}


std::string Locatable::getName()
{
    return name;
}

std::string Locatable::onExamine(LivingObject* o)
{
    std::cout << "examining" << std::endl;
    return name;
}

OptionList* Locatable::getOptionList(LivingObject* p)
{
    Library& library = Mayor::get().getLibrary();
    reg<Locatable> locatable = library.createLocatable(this);
    reg<OptionList> optionList = library.getOptionList(locatable.id());
    if(optionList.exists())
    {
        return optionList.get();
    }
    else
    {
        return library.createOptionList(new OptionList(),locatable.id()).get();
    }
}

void Locatable::setPosition(const Position& position)
{
    //bool change = !(position==position_);
    //if(change && position_.isValid())
    //    position_.getTile()->leave(this);
    if(position.map != position_.map)
    {
        if(position_.map) {
            Mayor::get().log << "Erase locatable from map" << std::endl;
            position_.map->locatables.erase(this);
        }
        if(position.map) {
            Mayor::get().log << "Add locatable to map" << std::endl;
            position.map->locatables.insert(this);
        }
    }
    position_ = position;
    //if(change && position_.isValid())
    //    position_.getTile()->enter(this);
}



}
