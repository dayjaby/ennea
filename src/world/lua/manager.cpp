#include "world/lua/manager.hpp"
#include "world/tile.hpp"
#include "world/map.hpp"
#include "world/optionlist.hpp"
#include "world/items.hpp"
#include "world/livingobject.hpp"

namespace Ennovia {
    namespace Lua {
        struct EntityWrapper : Entity, luabind::wrap_base
        {
            virtual OptionList* getOptionList(LivingObject* lo) {
                return call<OptionList*>("getOptionList",lo);
            }
            static OptionList* default_getOptionList(Entity* ptr, LivingObject* lo) {
                return ptr->Entity::getOptionList(lo);
            }
        };

        struct LocatableWrapper : Locatable, luabind::wrap_base
        {
            LocatableWrapper(const Position& position, const std::string& name_, const std::string& model_, const std::string& texture_)
            : Locatable(position,name_,model_,texture_) {
            }

            LocatableWrapper(const Position& position, const std::string& name_)
            : Locatable(position,name_) {
            }
        };

        struct LivingObjectWrapper : LivingObject, luabind::wrap_base
        {
            LivingObjectWrapper(const Position& position, const std::string& name_)
            : LivingObject(position,name_) {
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
            virtual void onChoose(LivingObject* performer) {
                call<void>("onChoose",performer);
            }
            static void default_onChoose(Option* ptr, LivingObject* performer) {
                ptr->Option::onChoose(performer);
            }
        };

        struct ItemTypeWrapper : ItemType, luabind::wrap_base
        {
            ItemTypeWrapper(const std::string& id,const std::string& name,const std::string& path)
            : ItemType(id,name,path) {}
            virtual bool isStackable() {
                return call<bool>("isStackable");
            }
            static bool default_isStackable(ItemType* ptr) {
                return ptr->ItemType::isStackable();
            }
            virtual std::string getName() {
                return call<std::string>("getName");
            }
            static std::string default_getName(ItemType* ptr) {
                return ptr->ItemType::getName();
            }
            virtual OptionList* getOptionList(LivingObject* lo,Item* i) {
                return call<OptionList*>("getOptionList",lo,i);
            }
            static OptionList* getOptionList(ItemType* ptr,LivingObject* lo,Item* i) {
                return ptr->ItemType::getOptionList(lo,i);
            }
        };

        Manager::Manager() : L(lua_open()) {
            loadlibs(L);
            init(L);
            if(luaL_loadfile(L,"lua/core.lua") || lua_pcall(L,0,0,0)) {
                std::cerr << "Cannot run lua file: " << lua_tostring(L,-1) << std::endl;
                throw 0;
            }
            std::cout << "Lua initialized" << std::endl;
        }
        Manager::~Manager() {
            lua_close(L);
        }
        lua_State* Manager::getState() { return L; }

        void Manager::openlualibs(lua_State* L, const luaL_reg* lib) {
            for(;lib->func;lib++) {
                lua_pushcfunction(L, lib->func);
                lua_pushstring(L, lib->name);
                lua_call(L, 1, 0);
            }
        }

        void Manager::loadlibs(lua_State* L) {
            static const luaL_reg lualibs[] =
                {{"base", luaopen_base},
                {NULL,NULL}};
            openlualibs(L,lualibs);
        }
        int Manager::add_file_and_line(lua_State* L)
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
        class Blub {
        public:
            typedef boost::multi_array<Tile,2> TileArray;
            typedef boost::multi_array<float,2> Heightmap;

            Blub(int id,const std::string& path_, const std::string& heightmappath_, int width_, int height_){}
            virtual ~Blub(){}

            std::string path,heightmappath;

        };
        void Manager::init(lua_State* L) {
            using namespace luabind;
            open(L);
            module(L)
            [
                class_<Blub>("Blub")
                    .def(constructor<int,const std::string&,const std::string&,int,int>()),
                class_<Position>("Position")
                    .def(constructor<>())
                    .def(constructor<float,float,Map*>())
                    .def_readwrite("x",&Position::x)
                    .def_readwrite("y",&Position::y)
                    .def_readwrite("map",&Position::map)
                , class_<Option,OptionWrapper>("Option")
                    .def("getDescription",&Option::getDescription,&OptionWrapper::default_getDescription)
                    .def("onChoose",&Option::onChoose,&OptionWrapper::default_onChoose)
                , class_<OptionList>("OptionList")
                    .def("add",&OptionList::add)
                    .def("addList",&OptionList::addList)
                , class_<Entity,EntityWrapper>("Entity")
                    .def(constructor<>())
                    .def("getOptionList", &Entity::getOptionList, &EntityWrapper::default_getOptionList)
                , class_<Locatable,Entity,LocatableWrapper>("Locatable")
                    .def(constructor<const Position&,const std::string&,const std::string&,const std::string&>())
                    .def(constructor<const Position&,const std::string&>())
                , class_<LivingObject,Locatable,LivingObjectWrapper>("LivingObject")
                    .def(constructor<const Position&,const std::string&>())
                , class_<Item,Entity>("Item")
                    .def("stack",&Item::stack)
                    .def_readwrite("count",&Item::count)
                , class_<ItemType,ItemTypeWrapper>("ItemType")
                    .def(constructor<const std::string&,const std::string&,const std::string&>())
                    .def("isStackable",&ItemType::isStackable,&ItemTypeWrapper::default_isStackable)
                    .def("getName",&ItemType::getName,&ItemTypeWrapper::default_getName)
                , class_<Map>("Map")
                    .def(constructor<int,const std::string&,const std::string&,int,int>())
                    .def("getHeight",&Map::getHeight)


                    /*class_<Buff, BuffWrapper>("Buff")
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
                    .def("onEffect", (void(Buff::*)(LivingObject*))&Buff::onEffect, &BuffWrapper::default_onEffect)
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
                    .def(constructor<const Position&, const std::string&>())
                    .def_readwrite("name", &Object::name)
                , class_<LivingObject, Object, LivingObjectWrapper>("LivingObject")
                    .def(constructor<const Position&, const std::string&>())
                    .def("applyBuff", &LivingObject::applyBuff, adopt(_2))
                    .def("isAlive", &LivingObject::isAlive)
                    .def("onDeath", &LivingObject::onDeath, &LivingObjectWrapper::default_onDeath)
                    .def("onDamage", &LivingObject::onDamage, &LivingObjectWrapper::default_onDamage)
                    .def("getStat", &LivingObject::getStat, dependency(result,_1))
                    .def("setStat", &LivingObject::setStat)
                , class_<Player, LivingObject, PlayerWrapper>("Player")
                    .def(constructor<const Position&, const std::string&>())
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
                    .def_readwrite("type", &Item::type)*/
            ];
        }
    }
}
