#ifndef ENNOVIA_MAYOR_HPP
#define ENNOVIA_MAYOR_HPP

#include <fstream>
#include "world/library.hpp"
#include "ui/render.hpp"
#include "network/client.hpp"
#include "world/actions.hpp"

namespace Ennovia {

class Mayor {
public:
    Mayor();

    static Mayor& get() {
        static Mayor instance;
        return instance;
    }

    bool alive;
    std::ofstream log;
    void run();

    /** tick() is called once every 0.1 seconds **/
    void tick();
    int getTicks() { return ticks; }

    void frame();

    Render& getRender() { return render; }
    Library& getLibrary() { return library; }
    reg<Player> getPlayer() { return localPlayer; }
    typedef std::set<Locatable*> LocatablesInAction;
    LocatablesInAction& getLocatablesInAction() { return locatablesInAction; }



    /** Interface for the view **/
    reg<irr::scene::ISceneNode> getLocatableSceneNode(Locatable* locatable);
    reg<Locatable> getSceneNodeLocatable(irr::scene::ISceneNode* node);

    void sendMessage(const std::string& msg);
    void getLocatableOptionList(int id);
    void walkTo(float x, float y);
    void chooseOption(int optionlist,int id);
    void getLocatablePosition(int id);
    void getLocatableIntroduction(int id);
    void login(const std::string& username, const std::string& password);
    // void sendAESKey...
    void requestMapData(int mapid);
    // ...

    /** Interface for the network **/
    void receiveMessage(const std::string& msg);
    void receiveLocatableOptionList(int id,OptionList* optionlist);
    void moveTo(int id,float x,float y);
    void onRequestCredentials();
    void onLoginUsernameInvalid();
    void onLoginPasswordInvalid();
    void onLoginValid();
    // void sendRSAKey...
    void youAre(int yourId);
    void setLocatablePosition(int id, int mapid, float x, float y);
    void introduceLocatable(int id, int type, const std::string& name);
    void setLocatableModel(int id, const std::string& model, const std::string& texture);
    void sendMapData(int id,const std::string& path, const std::string& heightmap, int width, int height);

private:
    LocatablesInAction locatablesInAction;

    boost::asio::io_service io_service;
    boost::asio::deadline_timer tick_timer;
    boost::asio::deadline_timer frame_timer;
    int ticks;

    /** The client is able to communicate with the server **/
    Client client;
    boost::shared_ptr<Connection> connection;
    reg<Player> localPlayer;

    /** Render **/
    Render render;

    /** Library **/
    Library library;
};


}

#endif

