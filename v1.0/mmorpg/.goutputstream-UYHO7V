
#include <string>
#include <iostream>
#include <luabind/adopt_policy.hpp>
#include "objects.hpp"

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


int main()
{
    lua_State* L = lua_open();
    loadlibs(L);
    init(L);
    if(luaL_loadfile(L,"objects.lua") || lua_pcall(L,0,0,0)) {
        std::cerr << "Cannot run configuration file: " << lua_tostring(L,-1) << std::endl;
        lua_close(L);
        return 0;
    }
    Object& warrior = * luabind::object_cast<Object*>(luabind::globals(L)["warrior"]);
    Object& bear = * luabind::object_cast<Object*>(luabind::globals(L)["bear"]);


	warrior.currentAction.id = 3;
	warrior.currentAction.victim = &bear;

	bear.currentAction.id = 3;
	bear.currentAction.victim = &warrior;

	Ability* poison = luabind::object_cast<Ability*>(luabind::globals(L)["poison"]);

    clock_t sixSecondTimer = clock() + 1 * CLOCKS_PER_SEC;

	while(warrior.isAlive() && bear.isAlive()) {
		if(warrior.currentAction.id == -1) {
			warrior.currentAction.id = 3;
			warrior.currentAction.victim = &bear;
		}
		if(clock()>sixSecondTimer) {
			warrior.currentAction.id = 2;
			warrior.currentAction.ability = poison;
			warrior.currentAction.victim = &bear;
			sixSecondTimer = clock() + 600 * CLOCKS_PER_SEC;
		}
		warrior.performAction();
		warrior.update();
		bear.performAction();
		bear.update();
	}

   /* {



	while(warrior.isAlive() && bear.isAlive()) {
		if(warrior.currentAction.id == -1) {
			warrior.currentAction.id = 3;
			warrior.currentAction.victim = &bear;
		}
		if(clock()>sixSecondTimer) {
			warrior.currentAction.id = 2;
			warrior.currentAction.ability = poison;
			warrior.currentAction.victim = &bear;
			sixSecondTimer = clock() + 6 * CLOCKS_PER_SEC;
		}
		warrior.performAction();
		warrior.update();
		bear.performAction();
		bear.update();
	}

    }*/
	std::cout << "before closing lua" << std::endl;
	lua_close(L);
	std::cout << "After closing lua" << std::endl;
	return 0;
}
