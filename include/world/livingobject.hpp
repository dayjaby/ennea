#ifndef ENNOVIA_LIVINGOBJECT_HPP
#define ENNOVIA_LIVINGOBJECT_HPP

#include "world/locatable.hpp"

namespace Ennovia
{
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

class LivingObject;

struct Ability
{
    Ability();
    virtual ~Ability();
    virtual void onCast(LivingObject* caster, LivingObject* victim);
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
    virtual void onEffectStart(LivingObject* carry);
    void onEffect(LivingObject* carry, int delta_ds);
    virtual void onEffect(LivingObject* carry);
    virtual void onEffectEnd(LivingObject* carry);
    virtual void onFullStacked(LivingObject* carry);
private:
    int ds;
};



class LivingObject : public Locatable
{
public:
    LivingObject(const Position& position, const std::string& name_);
    virtual ~LivingObject();

    virtual void onDamage(int amount);
    virtual void onDeath();

    void walkTo(const Position& target, float range = 0.1);
    bool isAlive();
    void recover();
    void updateBuffs();
    void applyBuff(Buff* buff);
    void setStat(const std::string& name, const Stat& s);
    const Stat& getStat(const std::string& name);

    virtual float getMovementSpeed();
 //   virtual bool isLiving() { return true; }
private:
    typedef std::map<std::string,Stat> Stats;
    typedef std::map<int,Buff*> Buffs;
    Stats stats;
    Buffs buffs;
    int lastBuffUpdate;
};
}

#endif
