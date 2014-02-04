#include "world/livingobject.hpp"
//#include "network/server.hpp" // to get ticks
#include "world/map.hpp"

namespace Ennovia{

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

    void Ability::onCast(LivingObject* caster, LivingObject* victim) {
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
    void Buff::onEffectStart(LivingObject* carry) {}
    void Buff::onEffect(LivingObject* carry, int delta_ds) {
        ds += delta_ds;
        while(ds >= effectCooldown) {
            ds -= effectCooldown;
            onEffect(carry);
        }
    }
    void Buff::onEffect(LivingObject* carry) {}
    void Buff::onEffectEnd(LivingObject* carry) {}
    void Buff::onFullStacked(LivingObject* carry) {}



   /// LivingObject

    LivingObject::LivingObject(const Position& position, const std::string& name_)
        : Locatable(position, name_), lastBuffUpdate(1234) { std::cout << "constr LivingObject" << std::endl; }

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

    void LivingObject::walkTo(const Position& pos, float range) {
        /*clearActionSchedule();
        Pathfinding pathfind(getPosition(),pos, range);
        const Position* current = &(getPosition());
        while(Position* next = pathfind.next()) {
            int dir = current->getDirection(*next);
            current = next;
            addToActionSchedule(new Move(pos.map,dir));
        }*/
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
        int current = 1234;
        int diff = current - lastBuffUpdate;
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

    void LivingObject::setStat(const std::string& name, const Stat& s) {
        stats[name] = s;
    }
    const Stat& LivingObject::getStat(const std::string& name) {
        return stats[name];
    }

    float LivingObject::getMovementSpeed() {
        return 0.25;
        //return getStat("movement").current / 10.0f;
    }
}
