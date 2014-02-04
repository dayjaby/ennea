#ifndef ENNOVIA_LIBRARY_HPP
#define ENNOVIA_LIBRARY_HPP

#include "misc/registry.hpp"
#include "core.hpp"

namespace Ennovia {


// custom delete a scene node
template <>
struct Deleter<irr::scene::ISceneNode*> {
    static void run(irr::scene::ISceneNode* x);
};
class Locatable;
class LivingObject;
class Player;
class Map;
class OptionList;
class Library {
public:
    reg<irr::scene::ISceneNode> createSceneNode(int id,irr::scene::ISceneNode* scene);
    reg<irr::scene::ISceneNode> getSceneNode(int id);
    reg<irr::scene::ISceneNode> getSceneNode(irr::scene::ISceneNode* node);
    reg<irr::scene::ISceneNode> getSceneNode(Locatable* loc);
    bool isSceneNode(int id);
    bool isSceneNode(irr::scene::ISceneNode* node);

    reg<Locatable> createLocatable(int id,const std::string& name);
    reg<Locatable> createLocatable(const std::string& name);
    reg<Locatable> createLocatable(Locatable* loc);
    reg<Locatable> getLocatable(int id);
    bool isLocatable(int id);

    reg<LivingObject> createLivingObject(int id,const std::string& name);
    reg<LivingObject> createLivingObject(const std::string& name);
    reg<LivingObject> createLivingObject(LivingObject* living);
    reg<LivingObject> getLivingObject(int id);
    bool isLivingObject(int id);

    reg<Player> createPlayer(int id,const std::string& name);
    reg<Player> createPlayer(const std::string& name);
    reg<Player> createPlayer(Player* player);
    reg<Player> getPlayer(int id);
    bool isPlayer(int id);

    reg<Map> getMap(int id);
    reg<Map> getMap(Map* map);
    reg<Map> createMap(Map* map,int id);
    bool isMap(int id);

    reg<OptionList> createOptionList(OptionList* list,int id);
    reg<OptionList> getOptionList(int id);
    bool isOptionList(int id);
};
}

#endif
