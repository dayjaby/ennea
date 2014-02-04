#include "world/library.hpp"
#include "world/locatable.hpp"
#include "world/livingobject.hpp"
#include "world/player.hpp"
#include "world/map.hpp"
#include "world/optionlist.hpp"

namespace Ennovia {

void Deleter<irr::scene::ISceneNode*>::run(irr::scene::ISceneNode* x) {
    //x->drop();
}

reg<irr::scene::ISceneNode> Library::createSceneNode(int id,irr::scene::ISceneNode* scene) {
    return reg<irr::scene::ISceneNode>(scene,id);
}

reg<irr::scene::ISceneNode> Library::getSceneNode(irr::scene::ISceneNode* node) {
    return reg<irr::scene::ISceneNode>(node);
}

reg<irr::scene::ISceneNode> Library::getSceneNode(int id) {
    return reg<irr::scene::ISceneNode>(id);
}

reg<irr::scene::ISceneNode> Library::getSceneNode(Locatable* loc) {
    reg<Locatable> locatable(loc);
    return reg<irr::scene::ISceneNode>(locatable.id());
}

bool Library::isSceneNode(int id) {
    return reg<irr::scene::ISceneNode>(id).exists();
}

bool Library::isSceneNode(irr::scene::ISceneNode* node) {
    return reg<irr::scene::ISceneNode>::exists(node);
}

reg<Locatable> Library::createLocatable(int id,const std::string& name) {
    Locatable* loc = new Locatable(Position(),name);
    return reg<Locatable>(loc,id);
}

reg<Locatable> Library::createLocatable(const std::string& name) {
    Locatable* loc = new Locatable(Position(),name);
    return reg<Locatable>(loc);
}

reg<Locatable> Library::createLocatable(Locatable* loc) {
    return reg<Locatable>(loc);
}

reg<Locatable> Library::getLocatable(int id) {
    return reg<Locatable>(id);
}

bool Library::isLocatable(int id) {
    return reg<Locatable>(id).exists();
}

reg<LivingObject> Library::createLivingObject(int id,const std::string& name) {
    LivingObject* lo = new LivingObject(Position(),name);
    reg<Locatable>(lo,id);
    return reg<LivingObject>(lo,id);
}

reg<LivingObject> Library::createLivingObject(const std::string& name) {
    LivingObject* lo = new LivingObject(Position(),name);
    reg<Locatable> loc(lo);
    return reg<LivingObject>(lo,loc.id());
}

reg<LivingObject> Library::createLivingObject(LivingObject* living) {
    reg<Locatable> loc(living);
    return reg<LivingObject>(living,loc.id());
}

reg<LivingObject> Library::getLivingObject(int id) {
    return reg<LivingObject>(id);
}

bool Library::isLivingObject(int id) {
    return reg<LivingObject>(id).exists();
}

reg<Player> Library::createPlayer(int id,const std::string& name) {
    Player* player = new Player(Position(),name);
    reg<Locatable>(player,id);
    reg<LivingObject>(player,id);
    return reg<Player>(player,id);
}

reg<Player> Library::createPlayer(const std::string& name) {
    Player* player = new Player(Position(),name);
    reg<Locatable> loc(player);
    reg<LivingObject> living(player,loc.id());
    return reg<Player>(player,loc.id());
}

reg<Player> Library::createPlayer(Player* player) {
    reg<Locatable> loc(player);
    reg<LivingObject> living(player,loc.id());
    return reg<Player>(player,loc.id());
}

reg<Player> Library::getPlayer(int id) {
    return reg<Player>(id);
}

bool Library::isPlayer(int id) {
    return reg<Player>(id).exists();
}

reg<Map> Library::getMap(int id) {
    return reg<Map>(id);
}

reg<Map> Library::getMap(Map* map) {
    return reg<Map>(map);
}

reg<Map> Library::createMap(Map* map,int id) {
    return reg<Map>(map,id);
}

bool Library::isMap(int id) {
    return reg<Map>(id).exists();
}

reg<OptionList> Library::createOptionList(OptionList* optionList,int id) {
    return reg<OptionList>(optionList,id);
}

reg<OptionList> Library::getOptionList(int id) {
    return reg<OptionList>(id);
}

bool Library::isOptionList(int id) {
    return reg<OptionList>(id).exists();
}

}
