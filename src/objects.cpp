#include "objects.hpp"
#include "gui.hpp"

namespace Ennovia {

    Stat::Stat() : current(0), max(0) {}
    Stat::Stat(int c) : current(c), max(c) {}
    Stat::Stat(int c, int m) : current(c), max(m) {}


    void Stat::increase(int amount) {
        if(amount<0) return;
        current += amount;
        if(current>max+5) current = max+5;
    }

    void Stat::decrease(int amount) {
        if(amount<0) return;
        current -= amount;
        if(current<0) current = 0;
    }

    void Stat::recover() {
        if(current>max) current--;
        if(current<max) current++;
    }



    Ability::Ability() : selfCastAllowed(false), friendlyCastAllowed(false) {
    }

    Ability::~Ability() {
    }

    void Ability::onCast(Object* caster, Object* victim) {
    }


    /// Buffs

    Buff::Buff()
    : permanent(true), visible(true), maxStacks(1), timeLeft(0), stacks(1), effectCooldown(1), ds(0)
    {
    }

    Buff::~Buff() {
    }

    void Buff::end() { timeLeft = 0; }
    int Buff::getID() { return 0; }
    void Buff::onEffectStart(Object* carry) {}
    void Buff::onEffect(Object* carry, int delta_ds) {
        ds += delta_ds;
        while(ds >= effectCooldown) {
            ds -= effectCooldown;
            onEffect(carry);
        }
    }
    void Buff::onEffect(Object* carry) {}
    void Buff::onEffectEnd(Object* carry) {}
    void Buff::onFullStacked(Object* carry) {}

    /// Object

    Object::Object(std::string name_) : name(name_) {
    }

    Object::~Object() {
    }

    OptionList Object::getOptionList(Player* p) {
        OptionList list;
        std::cout << "getting optionlist" << std::endl;
        list.add(new Examine<Object,Object>(this,this));
        return list;
    }

    std::string Object::getName() {
        return name;
    }

    std::string Object::onExamine(Object* o) {
        std::cout << "examining" << std::endl;
        return name;
    }

    /// LivingObject

    LivingObject::LivingObject(std::string name_)
        : Object(name_), lastBuffUpdate(clock()) { std::cout << "constr LivingObject" << std::endl; }

    LivingObject::~LivingObject() {
        for(Buffs::iterator i = buffs.begin() ; i != buffs.end() ; ++i) {
            delete(i->second);
        }
    }

    void LivingObject::onDamage(int amount) {
        stats["hp"].decrease(amount);
        if(!isAlive()) onDeath();
    }

    void LivingObject::onDeath() {}

    void LivingObject::walkTo(Position pos, float range) {
        clearActionSchedule();
        Pathfinding pathfind(getPosition(),pos, range);
        const Position* current = &(getPosition());
        while(Position* next = pathfind.next()) {
            Direction dir = current->getDirection(*next);
            current = next;
            addToActionSchedule(new Move(pos.map,dir));
        }
    }

    bool LivingObject::isAlive() { return stats["hp"].current>0; }

    void LivingObject::recover() {
        for(Stats::iterator i = stats.begin() ; i != stats.end() ; i++)
            i->second.recover();
    }

    void LivingObject::applyBuff(Buff* buff) {
        Buffs::iterator i = buffs.find(buff->getID());
        if(i != buffs.end()) {
            // Found, refresh the old buff and add one Item if necessary
            buff->stacks = i->second->stacks + 1;
            delete(i->second);
            buffs.erase(i);
        }
        buffs[buff->getID()] = buff;
        buff->onEffectStart(this);
        if(buff->stacks >= buff->maxStacks) {
            buff->stacks = buff->maxStacks;
            buff->onFullStacked(this);
        }
    }

    void LivingObject::updateBuffs() {
        clock_t current = clock();
        int diff = (current-lastBuffUpdate)*10 / CLOCKS_PER_SEC;
        if(diff) {
            for(Buffs::iterator i = buffs.begin(); i != buffs.end(); ++i) {
                Buff* buff = i->second;
                buff->timeLeft -= diff;
                if(buff->timeLeft > 0) {
                    buff->onEffect(this, diff);
                } else {
                    buff->onEffectEnd(this);
                    delete(i->second);
                    buffs.erase(i);
                }
            }
            lastBuffUpdate = current;
        }

    }

    void LivingObject::setStat(std::string name, const Stat& s) {
        stats[name] = s;
    }
    const Stat& LivingObject::getStat(std::string name) {
        return stats[name];
    }

    /// Player

    Player::Player(std::string name_)
        : LivingObject(name_), currentItem(0) {
    }

    bool Player::hasCurrentItem() {
        return currentItem;
    }




    void Player::onGainExp(std::string skillname, int amount) {
        	/*stat.exp = stat.exp + amount
	self:onGainExp(stat, amount)
	local x = stat.max
	while stat.exp > stat.exp_required do
		stat.max = stat.max + 1
		stat.current = stat.current + 1
		stat.exp = stat.exp - stat.exp_required
		stat.exp_required = stat.exp_required * 1.1
		self:onLevelup(stat)
	end
	return stat.max - x;*/
    }
    void Player::onLevelup(std::string skillname, int lvl) {}

    void TakeItemOption::onChoose(Player* p) {
        std::cout << "take item" << std::endl;
        switch(amount) {
            case -1:
                item.amount = 0;
                break;
            case 0:

                break;
            default:
                if(item.amount >= amount) {
                    item.amount -= amount;
                }
        }
        if(item.amount <= 0) {
            item.type = 0;
        }
        GUI::get()->updateGUIChest();
    }

    void OpenChestAction::onPerform(Entity* performer) {
        GUI::get()->createGUIChest(chest);
    }


    std::string OpenChestOption::getDescription() {
        return std::string("Open ") + chest->getName();
    }

    void OpenChestOption::onChoose(Player* p) {
        p->walkTo(chest->getPosition(),chest->getAccessDistance());
        p->addToActionSchedule(new OpenChestAction(chest));
    }

    OptionList Chest::getOptionList(Player* p) {
        OptionList ol;
        ol.add(new OpenChestOption(this));
        return ol.addList(Object::getOptionList(p));
    }

    int Chest::getWidth() { return 3; }
    int Chest::getHeight() { return 3; }
}
