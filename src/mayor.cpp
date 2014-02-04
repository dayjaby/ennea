#include "mayor.hpp"
#include "core.hpp"
#include "world/map.hpp"
#include "world/player.hpp"
#include "network/network.hpp"

namespace Ennovia {

Mayor::Mayor() :
    log("client_log.txt"),
    tick_timer(io_service, boost::posix_time::milliseconds(TICK)),
    frame_timer(io_service, boost::posix_time::milliseconds(FRAME)),
    //client(io_service,std::string("djabby.dlinkddns.com"),PORT_S),
    client(io_service,std::string("192.168.178.33"),PORT_S),
    connection(client.connection),
    render(1024,768,*this)
{
}

void Mayor::run() {
    alive = true;
    tick();
    frame();
    io_service.run();
}

void Mayor::tick()
{
    log << "Mayor.Tick" << std::endl;
    log << "<Tick " << std::flush;
    if(alive) {
        ticks++;
        log << "| alive | " << ticks << std::flush;
        tick_timer.expires_at(tick_timer.expires_at() + boost::posix_time::milliseconds(TICK));
        tick_timer.async_wait(boost::bind(&Mayor::tick, this));
        for(LocatablesInAction::iterator i = locatablesInAction.begin();i!=locatablesInAction.end();++i) {
            (*i)->actions.tick();
        }
    }
    log << ">" << std::endl;
}

void Mayor::frame()
{
    log << "Mayor.Frame" << std::endl;
    log << "<Frame " << std::flush;
    if(alive) {
        log << "| alive" << std::flush;
        frame_timer.expires_at(tick_timer.expires_at() + boost::posix_time::milliseconds(FRAME));
        frame_timer.async_wait(boost::bind(&Mayor::frame, this));

        if(render.run()) render.draw();
        log << "| draw " << std::flush;
    }
    log << ">" << std::endl;
}

/*
void Mayor::getTileOptionList(Tile& tile) {
    if(tile.getPosition().map) {
        reg<Map> map = library.getMap(tile.getPosition().map);
        GetTileOptionList msg;
        msg.set_map(map.id());
        msg.set_x(tile.getPosition().x);
        msg.set_y(tile.getPosition().y);
        client.write<GET_TILE_OPTIONLIST>(msg);
    }
}*/

void Mayor::getLocatableOptionList(int id) {
    GetLocatableOptionList msg;
    msg.set_id(id);
    client.write<GET_LOCATABLE_OPTIONLIST>(msg);
}

void Mayor::walkTo(float x, float y) {
    WalkTo msg;
    msg.set_x(x);
    msg.set_y(y);
    client.write<WALK_TO>(msg);
}

void Mayor::chooseOption(int optionlist,int id) {
    ChooseOption option;
    option.set_optionlist(optionlist);
    option.set_id(id);
    client.write<CHOOSE_OPTION>(option);
}

void Mayor::getLocatablePosition(int id) {
    GetLocatablePosition locatable;
    locatable.set_id(id);
    client.write<GET_LOCATABLE_POSITION>(locatable);
}

void Mayor::getLocatableIntroduction(int id) {
    GetLocatableIntroduction intro;
    intro.set_id(id);
    client.write<GET_LOCATABLE_INTRODUCTION>(intro);
    getLocatablePosition(id);
}

void Mayor::requestMapData(int mapid) {
    RequestMapData msg;
    msg.set_id(mapid);
    client.write<REQUEST_MAP>(msg);
}

void Mayor::receiveMessage(const std::string& msg) {
}
/*
void Mayor::receiveTileOptionList(int mapid,int x,int y,OptionList* optionlist) {
    log << "Receive tile option list" << std::endl;
    reg<Map> map(mapid);
    if(map.exists()) {
        log << "map exists" << std::endl;
        Position pos(x,y,map.get());
        if(pos.isValid()) {
            log << "position valid" << std::endl;
            Tile* tile = &map->getTile(x,y);
            render.processOptionList(tile,optionlist);
        }
    }
}
*/
void Mayor::receiveLocatableOptionList(int id,OptionList* optionlist) {
    reg<Locatable> locatable = library.getLocatable(id);
    if(locatable.exists()) {
        render.processOptionList(locatable.get(),optionlist);
    } else {
        getLocatableIntroduction(id);
    }
}

void Mayor::moveTo(int id,float x, float y) {
    reg<ISceneNode> sceneNode = library.getSceneNode(id);
    reg<Locatable> locatable = library.getLocatable(id);
    if(sceneNode.exists() && locatable.exists()) {
        render.moveSceneNode(locatable.get(),sceneNode.get(),x,y);
    } else {
        getLocatableIntroduction(id);
    }
}


void Mayor::onRequestCredentials() {
    Credentials credentials;
    std::cout << "Username: ";
    getline(std::cin,*credentials.mutable_username());
    std::cout << "Password: ";
    getline(std::cin,*credentials.mutable_password());
    client.write<CREDENTIALS>(credentials);
}


void Mayor::onLoginUsernameInvalid() {
    log << "Username invalid" << std::endl;
}

void Mayor::onLoginPasswordInvalid() {
    log << "Password invalid" << std::endl;
}

void Mayor::onLoginValid() {
    log << "Login successful!" << std::endl;
}

void Mayor::youAre(int yourId) {
    localPlayer = library.getPlayer(yourId);
    GetLocatableIntroduction intro;
    intro.set_id(yourId);
    client.write<GET_LOCATABLE_INTRODUCTION>(intro);
    log << "Local player: " << localPlayer.id() << std::endl;
}

void Mayor::setLocatablePosition(int id, int mapid, float x, float y) {
    log << "Set Locatable Position" << std::endl;
    reg<Locatable> locatable = library.getLocatable(id);
    if(locatable.exists()) {
        log << "Locatable exists" << std::endl;
        reg<Map> map = library.getMap(mapid);
        if(map.exists()) {
            log << "Map exists" << std::endl;
            log << locatable.id() << "," << localPlayer.id() << std::endl;
            log << reg<Map>(locatable->getPosition().map).id() << "," << map.id() << std::endl;
            log << "PMAP: " << reg<Map>(localPlayer->getPosition().map).id() << std::endl;
            Map* oldMap = locatable->getPosition().map;
            if(map.get()) {
                locatable->setPosition(Position(x,y,map));
                log << "Locatable repositioned" << std::endl;
                if(library.isSceneNode(id)) {
                    log << "Is scene node" << std::endl;
                    render.moveSceneNode(locatable.get(),library.getSceneNode(id).get(),x,y);
                    log << "Scene node moved" << std::endl;
                }
                if(locatable.id() == localPlayer.id() && oldMap != map.get()) {
                    log << "Render new map" << std::endl;
                    render.loadMap(map);
                }
            }

        } else {
            log << "Map doesnt exist" << std::endl;
            requestMapData(mapid);
            getLocatablePosition(id);
        }
    } else {
        log << "Locatable " << id << " doesnt exist" << std::endl;
        getLocatableIntroduction(id);
    }
    log << "Done"<< std::endl;
}

void Mayor::introduceLocatable(int id, int type, const std::string& name) {
    switch(type) {
    case 0:
        if(!library.isLocatable(id))
            library.createLocatable(id,name);
        break;
    case 1:
        if(!library.isLivingObject(id))
            library.createLivingObject(id,name);
        break;
    case 2:
        if(!library.isPlayer(id))
            library.createPlayer(id,name);
        break;
    }
}

void Mayor::setLocatableModel(int id, const std::string& model, const std::string& texture) {

    reg<ISceneNode> node = library.getSceneNode(id);
    if(!node.exists()) {
        irr::scene::ISceneNode* mesh = render.loadAnimatedMesh(model,texture);
        node = library.createSceneNode(id,mesh);
        getLocatablePosition(id);
    }
    if(id == localPlayer.id()) render.setCameraTarget(node.get());
}

void Mayor::sendMapData(int id,const std::string& path, const std::string& heightmap, int width, int height) {
    reg<Map> map = library.getMap(id);
    if(!map.exists())
        library.createMap(new Map(id,path,heightmap,width,height),id);
}


}

