#ifndef ENNOVIA_ITEMS_HPP
#define ENNOVIA_ITEMS_HPP

#include "world/entity.hpp"
#include "world/actions.hpp"
#include "misc/registry.hpp"

namespace Ennovia
{

class OptionList;
class Item;

class ItemType
{
public:
    ItemType(const std::string& id_,const std::string& name_, const std::string& path_) : name(name_), path(path_) {
        reg<ItemType,std::string>(this,id_);
    }

    virtual ~ItemType() {}
    virtual bool isStackable()
    {
        return false;
    }
    virtual std::string onExamine(Item* s)
    {
        return "The default ItemType";
    }
    virtual std::string getName()
    {
        return name;
    }
    const std::string& getPath()
    {
        return path;
    }
    virtual OptionList* getOptionList(LivingObject* o, Item* s);
private:
    std::string name;
    std::string path;
};


class Item : public Entity
{
public:
    Item()
        : type(reg<ItemType,std::string>()), count(0) {}

    Item(const std::string& itypename,int c)
        : type(reg<ItemType,std::string>(itypename)),count(c) {
    }

    Item(const std::string& itypename)
        : type(reg<ItemType,std::string>(itypename)),count(1) {
    }

    Item(const Item& other)
        : type(other.type), count(other.count)
    {

    }

    virtual ~Item() {
    }

    virtual OptionList* getOptionList(LivingObject* o);

    reg<ItemType,std::string> type;

    int count;
    bool stack(Item& item);

    bool exists() { return count>0; }
};

}
#endif
