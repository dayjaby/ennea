#include "gui.hpp"
#include "boost/tuple/tuple.hpp"

namespace Ennovia {

    Game* Game::game = 0;

    Game::Game(int width_, int height_)
    : width(width_), height(height_),
        cameraHeight(5), cameraAngle(90),
        optionListActive(false), optionListContextMenu(0),
        player("Player"),chest("Chest"),openChest(0),openChestElement(0)
    {
        guii = this;

        std::cout << "Device initialization" << std::endl;
        device = createDevice(video::EDT_OPENGL, dimension2d<u32>(width,height),16,false,false,false,&eventReceiver);
        guienv = device->getGUIEnvironment();
        driver = device->getVideoDriver();
        smgr = device->getSceneManager();
        collision = smgr->getSceneCollisionManager();
        std::cout << "Camera initialization" << std::endl;
        camera = smgr->addCameraSceneNode(0,vector3df(0,4.5,0),vector3df(25,0,25));
        entityParent = smgr->addEmptySceneNode();

        std::cout << "GUI initialization" << std::endl;

        /// GUI - Initialization

        eventReceiver.gui = guienv;
        device->setWindowCaption(L"Ennnovia - the ORPG");
        face.load("media/FreeMonoBold.ttf");
        fonts.resize(10);
        for(int i=0;i<10;i++) {
            fonts[i] = new CGUITTFont(driver);
            fonts[i]->attach(&face,12+i*2);
        }

        inventory = guienv->addWindow(core::rect<s32>(width-48*4-20,height-48*7-30,width,height),false,L"Inventory");
        inventory->setDraggable(false);
        inventory->getCloseButton()->setVisible(false);

        timer() = device->getTimer();

        std::cout << "Map initialization" << std::endl;

        currentMap = new Map(50,50);
        currentMap->getTile(1,0).type = TileType::Rock();
        currentMap->getTile(1,1).type = TileType::Rock();
        currentMap->getTile(1,2).type = TileType::Rock();
        currentMap->getTile(2,2).type = TileType::Rock();
        currentMap->getTile(3,2).type = TileType::Rock();
        currentMap->getTile(3,1).type = TileType::Rock();
        currentMap->getTile(3,0).type = TileType::Rock();
        for(int x=0;x<51;x++) {
            for(int y=0;y<51;y++) {
                currentMap->heightmap[x][y] = (x*x+y*y)/150.0;
            }
        }
        player.setPosition(Position(4,4,currentMap));
        chest.setPosition(Position(2,2,currentMap));
        Item& item = chest.getItemAt(0,0);
        item.type = lua.getItemType("Coins");
        item.amount = 117;

        std::cout << "3D initialization" << std::endl;


        mapx = new MapSceneNode(smgr->getRootSceneNode(), smgr, currentMap, 333);
        ITriangleSelector* selector = smgr->createTriangleSelectorFromBoundingBox(mapx);
        mapx->setTriangleSelector(selector);
        selector->drop();
        playerx = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/faerie.md2"),entityParent,1,core::vector3df(4.5,1,4.45));
        if(playerx) {
            selector = smgr->createTriangleSelectorFromBoundingBox(playerx);
            playerx->setMD2Animation(scene::EMAT_STAND);
            playerx->setMaterialTexture(0,driver->getTexture("media/faerie2.bmp"));
            playerx->setScale(vector3df(0.03,0.03,0.03));
            std::cout << playerx->getMesh()->getFrameCount() << std::endl;
            std::cout << playerx->getMesh()->getMeshType() << "/" << EAMT_MD2 << std::endl;
            playerx->setMaterialFlag(video::EMF_LIGHTING,false);
            playerx->setTriangleSelector(selector);
            selector->drop();
        }
        addEntity(&player,playerx);

        chestx = smgr->addMeshSceneNode(smgr->getMesh("media/treasure_chest.obj"),entityParent,1,core::vector3df(2.5,0,2.5));
        if(chestx) {
            selector = smgr->createTriangleSelectorFromBoundingBox(chestx);
            chestx->setMaterialTexture(0,driver->getTexture("media/treasure_chest.jpg"));
            chestx->setMaterialFlag(video::EMF_LIGHTING,false);
            chestx->setScale(vector3df(0.5,0.5,0.5));
            chestx->setRotation(vector3df(0,180,0));
            chestx->setTriangleSelector(selector);
            selector->drop();
        }
        addEntity(&chest,chestx);
        std::cout << "Register player: " << &player << "->" << playerx << std::endl;
    }

    Game::~Game() {
        device->drop();
    }

    void Game::run() {
        while(device->run()) {
            if(device->isWindowActive())
                onRun();
            else
                device->yield();
        }
    }

    void Game::onRun() {

   /*     updateCamera();
        fillOptionList();
        processOptionList();*/

        camera->setPosition(playerx->getPosition()+vector3df(cos(cameraAngle*PI/180)*6,cameraHeight,sin(cameraAngle*PI/180)*6));
        camera->setTarget(playerx->getPosition());
        if(eventReceiver.IsKeyDown(irr::KEY_LEFT)) {
            cameraAngle -= 0.5;
        }
        if(eventReceiver.IsKeyDown(irr::KEY_RIGHT)) {
            cameraAngle += 0.5;
        }
        if(eventReceiver.IsKeyDown(irr::KEY_UP)) {
                cameraHeight += 0.06;
        }
        if(eventReceiver.IsKeyDown(irr::KEY_DOWN)) {
            cameraHeight -= 0.06;
        }
        if(eventReceiver.IsKeyDown(irr::KEY_SPACE)) {
            playerx->setMD2Animation(scene::EMAT_ATTACK);
        }
        if(cameraHeight<0.1) cameraHeight = 0.1;
        if(cameraHeight>6) cameraHeight = 6;
        bool leftMouseClick = eventReceiver.mouseState.buttonClicked(MouseState::Left);
        bool rightMouseClick = eventReceiver.mouseState.buttonClicked(MouseState::Right);
        //if(leftMouseClick || rightMouseClick) {
            optionList.options.clear();
            IGUIElement* top = guienv->getRootGUIElement()->getElementFromPoint(eventReceiver.mouseState.Position);
            // is a gui element under the mouse cursor?
            if(top && top->getParent()) {
                // the gui element belongs to the chest?
                if(top->getParent()->getID() == DT_CHEST && top->getParent() != guienv->getRootGUIElement() && openChest) {
                    const boost::tuple<Chest*,int,int>& data = static_cast<GUIDataElement<boost::tuple<Chest*,int,int> > *>(top->getParent())->getData();
                    Item& item = data.get<0>()->getItemAt(data.get<1>(),data.get<2>());
                    ChestItem citem(data.get<0>(),data.get<1>(),data.get<2>());
                    optionList.addList(citem.getOptionList(&player));
                }
            } else { // if not, then get the 3d entities under the mouse cursor
                core::line3df ray = collision->getRayFromScreenCoordinates(eventReceiver.mouseState.Position,camera);
                core::vector3df intersection;
                core::triangle3df hitTriangle;
                if(ISceneNode* node = collision->getSceneNodeAndCollisionPointFromRay(ray,intersection,hitTriangle,0,mapx)) {
                    s32 id = node->getID();
                    int x = id % currentMap->width;
                    int y = (id - x) / currentMap->width;
                    optionList.addList(currentMap->getTile(x,y).getOptionList(&player));
                }
                if(ISceneNode* node = collision->getSceneNodeAndCollisionPointFromRay(ray,intersection,hitTriangle,0,entityParent)) {
                  //  ray.start = intersection + 0.000001 * (ray.end - ray.start);
                    Entity* entity = getNodesEntity(node);
                    if(entity != 0) {
                        optionList.addList(entity->getOptionList(&player));
                    }
                }
            }
        //}

        if(leftMouseClick) {
            if(optionListActive && currentOptionList.options.size()>0 && eventReceiver.contextMenuID != -1) {
                std::cout << "x" << std::endl;
                currentOptionList.options[eventReceiver.contextMenuID]->onChoose(&player);
                std::cout << "x" << std::endl;
                currentOptionList.options.clear();
                eventReceiver.contextMenuID = -1;
                optionListActive = false;
            } else if(optionList.options.size()>0) {
                optionList.options[0]->onChoose(&player);
            }
        }
        if(rightMouseClick) {
            currentOptionList = optionList;
            if(optionList.options.size()>0) {
                optionListActive = true;
                optionListContextMenu = guienv->addContextMenu(core::rect<s32>(eventReceiver.mouseState.Position,core::dimension2di(250,200)),0,-1);
                for(int i=0;i<optionList.options.size();i++) {
                    optionListContextMenu->addItem(core::stringw(optionList.options[i]->getDescription().c_str()).c_str());
                }
            }
        }

        if(openChest) {

        }


        driver->beginScene(true,true,SColor(255,100,101,140));
        smgr->drawAll();
        guienv->drawAll();
        player.updateActionSchedule();
        driver->endScene();
    }


    void GUI::onGUIElementClosed(IGUIElement* elem) {
        if(elem == openChestElement) {
            closeGUIChest();
        }
    }

    void GUI::updateInventory() {
    }

    void GUI::createGUIChest(Chest* chest) {
        if(chest == openChest) return;
        if(openChest) {
            closeGUIChest();
        }
        openChestElement = guienv->addWindow(core::rect<s32>(200,200,chest->getWidth()*48+204,chest->getHeight()*48+222),false,stringw(chest->getName().c_str()).c_str());
        openChestElement->getMinimizeButton()->setVisible(true);
        openChest = chest;
        updateGUIChest();
    }

    void GUI::updateGUIChest() {
        if(!openChest) return;
        core::list<IGUIElement*> children = openChestElement->getChildren();
        for(core::list<IGUIElement*>::Iterator i = children.begin(); i!=children.end();++i) {
            if((*i)->getID()>=0) (*i)->remove();
        }
        for(int x=0;x<openChest->getWidth();x++) {
            for(int y=0;y<openChest->getHeight();y++) {
                if(openChest->getItemAt(x,y).type) {
                    IGUIElement* dataElement =
                        new GUIDataElement<boost::tuple<Chest*,int,int> >(DT_CHEST,boost::tuple<Chest*,int,int>(openChest,x,y),core::rect<s32>(x*48+2,y*48+20,x*48+50,y*48+68),guienv,openChestElement);
                    IGUIImage* image = guienv->addImage(getTexture(openChest->getItemAt(x,y).type->getPath()),core::position2d<s32>(0,0),true,dataElement);
                    core::stringw word(openChest->getItemAt(x,y).amount);
                    int x2 = 48-fonts[2]->getWidthFromWord(word);
                    IGUIStaticText* text = guienv->addStaticText(word.c_str(),core::rect<s32>(x2,32,48,48),false,false,dataElement);
                    text->enableOverrideColor(true);
                    text->setOverrideColor(irr::video::SColor(255,255,0,0));
                    text->setOverrideFont(fonts[2]);
                }
            }
        }
    }

    void GUI::closeGUIChest() {
        std::cout << "Close open chest" << std::endl;
        openChest = 0;
        openChestElement->remove();
    }

    void Game::addEntity(Entity* entity, ISceneNode* node) {
        entityToNode[entity] = node;
        nodeToEntity[node] = entity;
    }

    Entity* Game::getNodesEntity(ISceneNode* node) {
        return nodeToEntity[node];
    }

    ISceneNode* Game::getEntitySceneNode(Entity* entity) {
        return entityToNode[entity];
    }

    void Game::removeEntity(Entity* entity) {
        entityToNode.erase(entityToNode.find(entity));
        nodeToEntity.erase(nodeToEntity.find(getEntitySceneNode(entity)));
    }
}
