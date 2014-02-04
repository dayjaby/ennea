#ifndef ENNOVIA_OBJECTS_HPP
#define ENNOVIA_OBJECTS_HPP

#include <string>
#include <algorithm>
#include <functional>
#include <map>
#include <sstream>
#include <ctime>
#include "world/entity.hpp"
#include "world/items.hpp"

namespace Ennovia
{
/*

class Chest : public Object
{
public:
    Chest(std::string name) : Object(name) {}
    virtual OptionList getOptionList(Player* p);
    virtual int getWidth();
    virtual int getHeight();
    float getAccessDistance()
    {
        return 1.42;
    }
    Item& getItemAt(int x, int y)
    {
        return items[y*getWidth()+x];
    }
    Item& getItemAt(int id)
    {
        return items[id];
    }
private:
    std::map<int,Item> items;
};

class TakeItemOption : public Option
{
public:
    TakeItemOption(Item& item_, int amount_) : item(item_), amount(amount_)
    {
    }

    virtual std::string getDescription()
    {
        std::stringstream ss;
        ss << "Take ";
        switch(amount)
        {
        case -1:
            ss << "all ";
            break;
        case 0:
            ss << "X ";
            break;
        default:
            ss << amount << ' ';
            break;
        }
        ss << item.type->getName();
        return ss.str();
    }


    virtual void onChoose(Player* p);
private:
    Item& item;
    int amount;
};

class ChestItem : public Entity
{
public:
    ChestItem(Chest* chest_, int x_, int y_)
        : chest(chest_), x(x_), y(y_)
    {
    }

    virtual OptionList getOptionList(Player* o)
    {
        OptionList ol;
        ol.add(new TakeItemOption(chest->getItemAt(x,y),-1));
        ol.add(new TakeItemOption(chest->getItemAt(x,y),0));
        ol.add(new TakeItemOption(chest->getItemAt(x,y),5));
        return ol;
    }
private:
    Chest* chest;
    int x,y;
};

class OpenChestAction : public Action
{
public:
    OpenChestAction(Chest* chest_) : chest(chest_) {}
    virtual int getTicks()
    {
        return 0;
    }
    virtual void onPerform(Entity* performer);
private:
    Chest* chest;
};

class OpenChestOption : public Option
{
public:
    OpenChestOption(Chest* chest_) : chest(chest_) {}
    virtual std::string getDescription();
    virtual void onChoose(Player* player);
private:
    Chest* chest;
};
}
*/
}
#endif
