#ifndef ENNOVIA_LUA_MANAGER_HPP
#define ENNOVIE_LUA_MANAGER_HPP

#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <luabind/dependency_policy.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/return_reference_to_policy.hpp>


namespace Ennovia {

    namespace Lua {
    /*struct ItemWrapper : ItemType, luabind::wrap_base
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
        virtual void onCast(LivingObject* caster, LivingObject* victim) {
            call<void>("onCast", caster, victim);
        }
        static void default_onCast(Ability* ptr, LivingObject* caster, LivingObject* victim) {
            ptr->Ability::onCast(caster, victim);
        }
    };

    struct BuffWrapper : Buff, luabind::wrap_base
    {
        virtual int getID() { return call<int>("getID"); }
        static int default_getID(Buff* ptr) { return ptr->Buff::getID(); }
        virtual void onEffectStart(LivingObject* carry) { call<void>("onEffectStart",carry); }
        static void default_onEffectStart(Buff* ptr, LivingObject* carry) { ptr->Buff::onEffectStart(carry); }
        virtual void onEffect(LivingObject* carry) { call<void>("onEffect",carry); }
        static void default_onEffect(Buff* ptr, LivingObject* carry) { ptr->Buff::onEffect(carry); }
        virtual void onEffectEnd(LivingObject* carry) { call<void>("onEffectEnd",carry); }
        static void default_onEffectEnd(Buff* ptr, LivingObject* carry) { ptr->Buff::onEffectEnd(carry); }
        virtual void onFullStacked(LivingObject* carry) { call<void>("onFullStacked",carry); }
        static void default_onFullStacked(Buff* ptr, LivingObject* carry) { ptr->Buff::onFullStacked(carry); }
    };

    struct EntityWrapper : Entity, luabind::wrap_base
    {
        virtual OptionList* getOptionList() {
            return call<OptionList*>("getOptionList");
        }
        static OptionList* default_getOptionList(Entity* ptr, Player* user) {
            return ptr->Entity::getOptionList(user);
        }
    };

    struct LivingObjectWrapper : LivingObject, luabind::wrap_base
    {
        LivingObjectWrapper(const Position& position, const std::string& name_) : LivingObject(position, name_) {}

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
        PlayerWrapper(const Position& position, const std::string& name_) : Player(position, name_) {}

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

    };*/

    class Manager
    {
    public:
        Manager();
        ~Manager();
        lua_State* getState();
    private:
        void openlualibs(lua_State* L, const luaL_reg* lib);
        void loadlibs(lua_State* L);
        static int add_file_and_line(lua_State* L);
        void init(lua_State* L);

        lua_State* L;
    };

    }
}
#endif
