#ifndef ENNOVIA_GAME_HPP
#define ENNOVIA_GAME_HPP

#include "objects.hpp"
#include "graphics.hpp"
#include "lua_manager.hpp"
#include "eventreceiver.hpp"
#include "CGUITTFont.h"

namespace Ennovia {

    class Game {
    public:
        Game(int width_, int height_);
        ~Game();

        static Game* get() {
            return game;
        }


        void run();

        irr::video::IVideoDriver*& getDriver() { return driver; }
        // creates a window that shows the chest contents
        Player& getPlayer() { return player; }
        void onGUIElementClosed(IGUIElement* elem);
        void updateInventory();
        void createGUIChest(Chest* chest);
        void updateGUIChest();
        void closeGUIChest();

        void addEntity(Entity* entity,ISceneNode* node);
        Entity* getNodesEntity(ISceneNode* node);
        ISceneNode* getEntitySceneNode(Entity* entity);
        void removeEntity(Entity* entity);

        irr::video::ITexture* getTexture(std::string name) {
            if(textures.find(name) == textures.end()) {
                textures[name] = driver->getTexture(name.c_str());
            }
            return textures[name];
        }
    private:
        void onRun();
        int width, height;
        MyEventReceiver eventReceiver;
        IrrlichtDevice* device;
        LuaManager lua;

        static Game* game;

        irr::gui::IGUIEnvironment* guienv;
        irr::video::IVideoDriver* driver;
        irr::scene::ISceneManager* smgr;
        scene::ISceneCollisionManager* collision;

        float cameraHeight, cameraAngle;
        bool optionListActive;
        IGUIContextMenu* optionListContextMenu;
        OptionList optionList;
        OptionList currentOptionList;

        scene::ICameraSceneNode* camera;
        scene::ISceneNode* entityParent;
        scene::IAnimatedMeshSceneNode* playerx;
        scene::IMeshSceneNode* chestx;
        MapSceneNode* mapx;

        Map* currentMap;
        CGUITTFace face;
        std::vector<CGUITTFont*> fonts;

        std::map<Entity*,ISceneNode*> entityToNode;
        std::map<ISceneNode*,Entity*> nodeToEntity;

        irr::gui::IGUIWindow* inventory;
        std::map<std::string,irr::video::ITexture*> textures;
        Player player;
        Chest chest;



    };

}

#endif
