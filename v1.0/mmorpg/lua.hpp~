#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

#include "objects.hpp"


struct AbilityWrapper : Ability, luabind::wrap_base
{
    virtual void onCast(Object* caster, Object* victim) {
        call<void>("onCast", caster, victim);
    }
    static void default_onCast(Ability* ptr, Object* caster, Object* victim) {
        ptr->Ability::onCast(caster, victim);
    }
};

struct ActionWrapper : Action, luabind::wrap_base
{
    virtual void onPerform(Object* performer) {
        call<void>("onPerform", performer);
    }
    static void default_onPerform(Action* ptr, Object* p) {
        ptr->Action::onPerform(ptr,p);
    }
};

struct BuffWrapper : Buff, luabind::wrap_base
{
    virtual int getID() { return call<int>("getID"); }
    static int default_getID(Buff* ptr) { return ptr->Buff::getID(); }
    virtual void onEffectStart(Object* carry) { call<void>("onEffectStart",carry); }
    static void default_onEffectStart(Buff* ptr, Object* carry) { ptr->Buff::onEffectStart(carry); }
    virtual void onEffect(Object* carry) { call<void>("onEffect",carry); }
    static void default_onEffect(Buff* ptr, Object* carry) { ptr->Buff::onEffect(carry); }
    virtual void onEffectEnd(Object* carry) { call<void>("onEffectEnd",carry); }
    static void default_onEffectEnd(Buff* ptr, Object* carry) { ptr->Buff::onEffectEnd(carry); }
    virtual void onFullStacked(Object* carry) { call<void>("onFullStacked",carry); }
    static void default_onFullStacked(Buff* ptr, Object* carry) { ptr->Buff::onFullStacked(carry); }
};


struct ObjectWrapper : Object, luabind::wrap_base
{
    virtual void onDamage(DamageType type, int amount) {
        call<void>("onDamage", type, amount);
    }
    static void default_onDamage(Object* ptr, DamageType type, int amount) {
        ptr->Object::onDamage(type,amount);
    }
    virtual void onDeath() {
        call<void>("onDeath");
    }
    static void default_onDeath(Object* ptr) {
        ptr->Object::onDeath();
    }
    virtual void onGainExp(std::string skillname, int amount) {
        call<void>("onGainExp", skillname, amount);
    }
    static void default_onGainExp(Object* ptr, std::string skillname, int amount) {
        ptr->Object::onGainExp(skillname, amount);
    }
    virtual void onLevelup(std::string skillname, int amount) {
        call<void>("onLevelup", skillname, amount);
    }
    static void default_onLevelup(Object* ptr, std::string skillname, int amount) {
        ptr->Object::onLevelup(skillname, amount);
    }
};



extern "C" void init() {
    using namespace luabind;
    open(L);



    module(L)
    [
        class_<DamageType>("DamageType")
            .scope
            [
                def("Physical", &DamageType::Physical),
                def("Fire", &DamageType::Fire),
                def("Poison", &DamageType::Poison),
                def("Ice", &DamageType::Ice)
            ]
            .def("name", &DamageType::name)
            .def(const_self == other<DamageType>())
            .def(const_self < other<DamageType>())
        , class_<Buff, BuffWrapper>("Buff")
            .def(constructor<>())
            .def_readwrite("visible", &Buff::visible)
            .def_readwrite("permanent", &Buff::permanent)
            .def_readwrite("maxStacks", &Buff::maxStacks)
            .def_readwrite("timeLeft", &Buff::timeLeft)
            .def_readwrite("stacks", &Buff::stacks)
            .def_readwrite("effectCooldown", &Buff::effectCooldown)
            .def("end", &Buff::end)
            .def("getID", &Buff::getID, &BuffWrapper::default_getID)
            .def("onEffectStart", &Buff::onEffectStart, &BuffWrapper::default_onEffectStart)
            .def("onEffect", (void(Buff::*)(Object*))&Buff::onEffect, &BuffWrapper::default_onEffect)
            .def("onEffectEnd", &Buff::onEffectEnd, &BuffWrapper::default_onEffectEnd)
            .def("onFullStacked", &Buff::onFullStacked, &BuffWrapper::default_onFullStacked)
        , class_<Ability, AbilityWrapper>("Ability")
            .def(constructor<>())
            .def("onCast", &Ability::onCast, &AbilityWrapper::default_onCast)
            .def_readwrite("selfCastAllowed", &Ability::selfCastAllowed)
            .def_readwrite("friendlyCastAllowed", &Ability::friendlyCastAllowed)
        , class_<Action, ActionWrapper>("Action")
            .def("onPerform", &Action::onPerform, &ActionWrapper::default_onPerform)
        , class_<Stat>("Stat")
            .def(constructor<>())
            .def(constructor<int>())
            .def(constructor<int,int>())
            .def_readwrite("current", &Stat::current)
            .def_readwrite("max", &Stat::max)
            .def_readwrite("exp", &Stat::exp)
            .def_readwrite("exp_required", &Stat::exp_required)
            .def("increase", &Stat::increase)
            .def("recover", &Stat::recover)
            .def("learn", &Stat::learn<Object>)
        , class_<Object, ObjectWrapper>("Object")
            .def(constructor<>())
            .def("getResistance", &Object::getResistance)
            .def("setResistance", &Object::setResistance)
            .def("applyBuff", &Object::applyBuff)
            .def("isAlive", &Object::isAlive)
            .def("onDeath", &Object::onDeath, &ObjectWrapper::default_onDeath)
            .def("onDamage", &Object::onDamage, &ObjectWrapper::default_onDamage)
            .def("onGainExp", &Object::onGainExp, &ObjectWrapper::default_onGainExp)
            .def("onLevelup", &Object::onLevelup, &ObjectWrapper::default_onLevelup)
            .def("damage", &Object::damage)
            .def_readwrite("name", &Object::name)
            .def_readwrite("hp", &Object::hp)
            .def_readwrite("strength", &Object::strength)
    ];
}
