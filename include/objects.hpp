#ifndef ENNOVIA_OBJECTS_HPP
#define ENNOVIA_OBJECTS_HPP

#include <string>
#include <algorithm>
#include <functional>
#include <map>
#include <sstream>
#include <ctime>
#include "entity.hpp"
#include "items.hpp"

namespace Ennovia {

    struct Stat
    {
        int current, max;
        int exp, exp_required;
        Stat();
        Stat(int c);
        Stat(int c, int m);
        void increase(int amount);
        void decrease(int amount);
        void recover();
    };

    class Object;

    struct Ability
    {
        Ability();
        virtual ~Ability();
        virtual void onCast(Object* caster, Object* victim);
        bool selfCastAllowed, friendlyCastAllowed;
    };

    class Buff
    {
    public:
        Buff();
        virtual ~Buff();
        bool permanent;
        bool visible;
        int maxStacks;
        int timeLeft; // *0.1 seconds
        int stacks;
        int effectCooldown;
        void end();
        virtual int getID();
        virtual void onEffectStart(Object* carry);
        void onEffect(Object* carry, int delta_ds);
        virtual void onEffect(Object* carry);
        virtual void onEffectEnd(Object* carry);
        virtual void onFullStacked(Object* carry);
    private:
        int ds;
    };

    class Object : public Entity
    {
    public:
        Object(std::string name_);
        virtual ~Object();

        virtual OptionList getOptionList(Player* p);
        virtual std::string getName();
        virtual std::string onExamine(Object* o);
        std::string name;
    };

    class LivingObject : public Object
    {
    public:
        LivingObject(std::string name_);
        virtual ~LivingObject();

        virtual void onDamage(int amount);
        virtual void onDeath();

        void walkTo(Position target, float range = 0.1);
        bool isAlive();
        void recover();
        void updateBuffs();
        void applyBuff(Buff* buff);
        void setStat(std::string name, const Stat& s);
        const Stat& getStat(std::string name);

    private:
        typedef std::map<std::string,Stat> Stats;
        typedef std::map<int,Buff*> Buffs;
        Stats stats;
        Buffs buffs;
        time_t lastBuffUpdate;
    };

    class Player : public LivingObject
    {
    public:
        Player(std::string name_);
        Item* currentItem;
        bool hasCurrentItem();

        virtual void onGainExp(std::string skillname, int amount);
        virtual void onLevelup(std::string skillname, int lvl);

    };

    class Chest : public Object {
    public:
        Chest(std::string name) : Object(name) {}
        virtual OptionList getOptionList(Player* p);
        virtual int getWidth();
        virtual int getHeight();
        float getAccessDistance() { return 1.42; }
        Item& getItemAt(int x, int y) {
            return items[y*getWidth()+x];
        }
        Item& getItemAt(int id) {
            return items[id];
        }
    private:
        std::map<int,Item> items;
    };

    class TakeItemOption : public Option {
    public:
        TakeItemOption(Item& item_, int amount_) : item(item_), amount(amount_) {
        }

        virtual std::string getDescription() {
            std::stringstream ss;
            ss << "Take ";
            switch(amount) {
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

    class ChestItem : public Entity {
    public:
        ChestItem(Chest* chest_, int x_, int y_)
        : chest(chest_), x(x_), y(y_) {
        }

        virtual OptionList getOptionList(Player* o) {
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

    class OpenChestAction : public Action {
    public:
        OpenChestAction(Chest* chest_) : chest(chest_) {}
        virtual int getTicks() { return 0; }
        virtual void onPerform(Entity* performer);
    private:
        Chest* chest;
    };

    class OpenChestOption : public Option {
    public:
        OpenChestOption(Chest* chest_) : chest(chest_) {}
        virtual std::string getDescription();
        virtual void onChoose(Player* player);
    private:
        Chest* chest;
    };
}

#endif
