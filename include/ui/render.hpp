#ifndef ENNOVIA_RENDER_HPP
#define ENNOVIA_RENDER_HPP

#include "misc/registry.hpp"
#include "misc/irrlicht.hpp"
#include "misc/CGUITTFont.h"
#include "ui/graphics.hpp"
#include "ui/eventreceiver.hpp"
#include "world/position.hpp"
#include "world/optionlist.hpp"

namespace Ennovia {
    class Map;
    class Mayor;
    class Library;
    class Locatable;
    class Render {
    public:
        Render(int width_, int height_, Mayor& mayor_);
        ~Render();
        void loadMap(Map* map);
        void setMap(irr::scene::ISceneNode* node, irr::scene::ISceneNode* map);

        void processOptionList(Locatable* locatable,OptionList* optionlist);
        irr::scene::ISceneNode* loadAnimatedMesh(const std::string& path, const std::string& texture);
        irr::video::ITexture* getTexture(const std::string& texture);
        void setCameraTarget(ISceneNode* node);
        void setAnimation(ISceneNode* node,EMD2_ANIMATION_TYPE animation);
        void moveSceneNode(Locatable* locatable,irr::scene::ISceneNode* node,float x, float y);
        float getMapHeight(float x, float y);
        void draw();
        bool run();

        void moveMouse(int x,int y,bool left,bool right);
        void updateOptionList();
        void mouseLeftClick(int x,int y);
        void mouseRightClick(int x,int y);
        void moveCamera(float x,float y,float z);
        irr::gui::IGUIEnvironment* getGUIEnvironment() {return guienv; }
        irr::video::IVideoDriver* getVideoDriver() { return driver; }

        irr::core::rect<s32> getTopLeftRect(int w,int h);
        irr::core::rect<s32> getTopRightRect(int w,int h);
        irr::core::rect<s32> getBottomLeftRect(int w,int h);
        irr::core::rect<s32> getBottomRightRect(int w,int h);
    private:
        Position lastMousePosition;
        irr::core::position2di lastMouseLocation;
        Position getPositionUnderMouse(int mx,int my);
        void getLocatablesUnderMouse(std::vector<Locatable*>* locatables,int mx,int my);
        void processLeftClick();
        void processRightClick();

        Mayor& mayor;
        Library& library;

        int width, height;
        EventReceiver eventReceiver;
        IrrlichtDevice* device;
        irr::gui::IGUIEnvironment* guienv;
        irr::video::IVideoDriver* driver;
        irr::scene::ISceneManager* smgr;
        scene::ISceneCollisionManager* collision;

        float cameraHeight, cameraAngle, cameraDistance;
        ISceneNode* cameraTarget;

        scene::ICameraSceneNode* camera;
        scene::ISceneNode* entityParent;
        ISceneNode* map;

        reg<Map> currentMap;
        std::map<std::string,irr::video::ITexture*> textures;

        OptionList optionList;
        irr::gui::IGUIContextMenu* optionListContextMenu;
        std::map<Locatable*,OptionList*> optionLists;
        std::vector<Locatable*> leftClick;
        std::vector<Locatable*> rightClick;

        irr::gui::IGUIWindow* inventory;

    };

    #define RENDER Mayor::get().getRender()

/*
    class GUI {
    public:
        GUI(int width_, int height_);
        ~GUI();

        static GUI* get() {
            return gui;
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

        static GUI* gui;

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
        Player player;*/
//        Chest chest;



  //  };

}

#endif
