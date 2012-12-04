#ifndef ENNOVIA_LUA_MANAGER_HPP
#define ENNOVIE_LUA_MANAGER_HPP

#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <luabind/dependency_policy.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/return_reference_to_policy.hpp>
#include "objects.hpp"
#include "items.hpp"

namespace Ennovia {

    struct ItemWrapper : ItemType, luabind::wrap_base
    {
        ItemWrapper(std::string name_, std::string path_) : ItemType(name_, path_) {}
        virtual bool isStackable() {
            return call<bool>("isStackable");
        }
        static bool default_isStackable(ItemType* ptr) {
            return ptr->ItemType::isStackable();
        }
        virtual std::string onExamine(Item* s) {
            return call<std::string>("onExamine",s);
        }
        static std::string default_onExamine(ItemType* ptr, Item* s) {
            return ptr->ItemType::onExamine(s);
        }
        virtual std::string getName() {
            return call<std::string>("getName");
        }
        static std::string default_getName(ItemType* ptr) {
            return ptr->ItemType::getName();
        }
        virtual OptionList getOptionList(Player* o, Item* s) {
            return call<OptionList>("getOptionList", o, s);
        }
        static OptionList default_getOptionList(ItemType* ptr, Player* o, Item* s) {
            return ptr->ItemType::getOptionList(o, s);
        }
    };

    struct OptionWrapper : Option, luabind::wrap_base
    {
        virtual std::string getDescription() {
            return call<std::string>("getDescription");
        }
        static std::string default_getDescription(Option* ptr) {
            return ptr->Option::getDescription();
        }
        virtual void onChoose(Player* performer) {
            call<void>("onPerform",performer);
        }
        static void default_onChoose(Option* ptr, Player* performer) {
            ptr->Option::onChoose(performer);
        }
    };

    struct ActionWrapper : Action, luabind::wrap_base
    {
        virtual int getTicks() {
            return call<int>("getTicks");
        }

        static int default_getTicks(Action* ptr) {
            return ptr->Action::getTicks();
        }

        virtual void onPerform(LivingObject* performer) {
            call<void>("onPerform",performer);
        }
        static void default_onPerform(Action* ptr, LivingObject* performer) {
            ptr->Action::onPerform(performer);
        }
    };


    struct AbilityWrapper : Ability, luabind::wrap_base
    {
        virtual void onCast(Object* caster, Object* victim) {
            call<void>("onCast", caster, victim);
        }
        static void default_onCast(Ability* ptr, Object* caster, Object* victim) {
            ptr->Ability::onCast(caster, victim);
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

    struct EntityWrapper : Entity, luabind::wrap_base
    {
        virtual OptionList getOptionList() {
            return call<OptionList>("getOptionList");
        }
        static OptionList default_getOptionList(Entity* ptr, Player* user) {
            return ptr->Entity::getOptionList(user);
        }
    };

    struct ObjectWrapper : Object, luabind::wrap_base
    {
        ObjectWrapper(std::string name_) : Object(name_) {}
    };

    struct LivingObjectWrapper : LivingObject, luabind::wrap_base
    {
        LivingObjectWrapper(std::string name_) : LivingObject(name_) {}

        virtual void onDamage(int amount) {
            call<void>("onDamage", amount);
        }
        static void default_onDamage(LivingObject* ptr, int amount) {
            ptr->LivingObject::onDamage(amount);
        }
        virtual void onDeath() {
            call<void>("onDeath");
        }
        static void default_onDeath(LivingObject* ptr) {
            ptr->LivingObject::onDeath();
        }
    };

    struct PlayerWrapper : Player, luabind::wrap_base
    {
        PlayerWrapper(std::string name_) : Player(name_) {}

        virtual void onGainExp(std::string skillname, int amount) {
            call<void>("onGainExp", skillname, amount);
        }
        static void default_onGainExp(Player* ptr, std::string skillname, int amount) {
            ptr->Player::onGainExp(skillname, amount);
        }
        virtual void onLevelup(std::string skillname, int amount) {
            call<void>("onLevelup", skillname, amount);
        }
        static void default_onLevelup(Player* ptr, std::string skillname, int amount) {
            ptr->Player::onLevelup(skillname, amount);
        }

    };

    class LuaManager
    {
    public:
        LuaManager() : L(lua_open()) {
            loadlibs(L);
            init(L);
            if(luaL_loadfile(L,"lua/core.lua") || lua_pcall(L,0,0,0)) {
                std::cerr << "Cannot run lua file: " << lua_tostring(L,-1) << std::endl;
                throw 0;
            }
            std::cout << "Lua initialized" << std::endl;
        }
        ~LuaManager() {
            lua_close(L);
        }
        lua_State* getState() { return L; }

        ItemType* getItemType(std::string name) {
            return luabind::object_cast<ItemType*>(luabind::globals(L)["ItemTypes"][name]);
        }
    private:
        void openlualibs(lua_State* L, const luaL_reg* lib) {
            for(;lib->func;lib++) {
                lua_pushcfunction(L, lib->func);
                lua_pushstring(L, lib->name);
                lua_call(L, 1, 0);
            }
        }

        void loadlibs(lua_State* L) {
            static const luaL_reg lualibs[] =
                {{"base", luaopen_base},
                {NULL,NULL}};
            openlualibs(L,lualibs);
        }
        static int add_file_and_line(lua_State* L)
        {
           lua_Debug d;
           lua_getstack(L, 1, &d);
           lua_getinfo(L, "Sln", &d);
           std::string err = lua_tostring(L, -1);
           lua_pop(L, 1);
           std::stringstream msg;
           msg << d.short_src << ":" << d.currentline;

           if (d.name != 0)
           {
              msg << "(" << d.namewhat << " " << d.name << ")";
           }
           msg << " " << err;
           lua_pushstring(L, msg.str().c_str());
           return 1;
        }
        void init(lua_State* L) {
            using namespace luabind;
            open(L);
            module(L)
            [
                class_<Buff, BuffWrapper>("Buff")
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
                , class_<Action,ActionWrapper>("Action")
                    .def(constructor<>())
                    .def("getTicks", &Action::getTicks, &ActionWrapper::default_getTicks)
                    .def("onPerform", &Action::onPerform, &ActionWrapper::default_onPerform)
                , class_<Option>("Option")
                    .def(constructor<>())
                    .def("getDescription",&Option::getDescription, &OptionWrapper::default_getDescription)
                    .def("onChoose",&Option::onChoose, &OptionWrapper::default_onChoose)
                , class_<OptionList>("OptionList")
                    .def(constructor<>())
                    .def("add", &OptionList::add, adopt(_2))
                    .def("addList", &OptionList::add)
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
                , class_<Entity, EntityWrapper>("Entity")
                    .def(constructor<>())
                    .def("getOptionList", &Entity::getOptionList, &EntityWrapper::default_getOptionList)
                , class_<Object, Entity, ObjectWrapper>("Object")
                    .def(constructor<std::string>())
                    .def_readwrite("name", &Object::name)
                , class_<LivingObject, Object, LivingObjectWrapper>("LivingObject")
                    .def("applyBuff", &LivingObject::applyBuff, adopt(_2))
                    .def("isAlive", &LivingObject::isAlive)
                    .def("onDeath", &LivingObject::onDeath, &LivingObjectWrapper::default_onDeath)
                    .def("onDamage", &LivingObject::onDamage, &LivingObjectWrapper::default_onDamage)
                    .def("getStat", &LivingObject::getStat, dependency(result,_1))
                    .def("setStat", &LivingObject::setStat)
                , class_<Player, LivingObject, PlayerWrapper>("Player")
                    .def("onGainExp", &Player::onGainExp, &PlayerWrapper::default_onGainExp)
                    .def("onLevelup", &Player::onLevelup, &PlayerWrapper::default_onLevelup)
                    .def("hasCurrentItem", &Player::hasCurrentItem)
                    .def_readwrite("currentItem", &Player::currentItem)
                , class_<ItemType,ItemWrapper>("ItemType")
                    .def(constructor<std::string,std::string>())
                    .def("isStackable", &ItemType::isStackable, &ItemWrapper::default_isStackable)
                    .def("onExamine", &ItemType::onExamine, &ItemWrapper::default_onExamine)
                    .def("getName", &ItemType::getName, &ItemWrapper::default_getName)
                    .def("getPath", &ItemType::getPath)
                    .def("getOptionList", &ItemType::getOptionList, &ItemWrapper::default_getOptionList)
                , class_<Item>("Item")
                    .def(constructor<ItemType*>())
                    .def(constructor<ItemType*, int>())
                    .def(constructor<ItemType*, int, int>())
                    .def("changeItemType", &Item::changeItemType)
                    .def_readwrite("amount", &Item::amount)
                    .def_readwrite("quality", &Item::quality)
                    .def_readwrite("type", &Item::type)
            ];
        }
        lua_State* L;
    };



}
#endif
