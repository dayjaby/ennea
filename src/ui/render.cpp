#include "ui/render.hpp"
#include "world/optionlist.hpp"
#include "world/player.hpp"
#include "world/map.hpp"
#include <boost/tuple/tuple.hpp>
#include "mayor.hpp"
#include "ui/guiinventory.hpp"

namespace Ennovia
{

Render::Render(int width_, int height_,Mayor& mayor_) :
    width(width_), height(height_), mayor(mayor_), library(mayor_.getLibrary())
{
    device = createDevice(video::EDT_OPENGL, dimension2d<u32>(width,height),16,false,false,false);


    guienv = device->getGUIEnvironment();
    eventReceiver.gui = guienv;
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    collision = smgr->getSceneCollisionManager();
    device->getFileSystem()->addFileArchive("media");
    device->setEventReceiver(&eventReceiver);
    moveCamera(0,0,0);

    camera = smgr->addCameraSceneNode(0,vector3df(0,20.f,0),vector3df(25,0,25));

    entityParent = smgr->addEmptySceneNode();

    //loadMap("chessboard.3ds","",50,50);
    //loadMap("chessboard.3ds","",50,50);
    // camera->setTarget(node->getBoundingBox().getCenter());
//        mayor.log <<  << "," << node->getBoundingBox().getExtent().Y << ","<< node->getBoundingBox().getExtent().Z << ","<< std::endl;

}

Render::~Render()
{
    if(map) map->drop();
    device->drop();
}
void Render::draw()
{
    video::SMaterial material;
    material.Lighting = false;
    material.Wireframe = false;

    if(cameraTarget)
    {
        mayor.log << "CT" << std::flush;
        float theta = cameraHeight*PI/180;
        float phi = cameraAngle*PI/180;
        camera->setPosition(cameraTarget->getPosition()
                        +vector3df(sin(theta)*cos(phi)*cameraDistance,
                                   cos(theta)*cameraDistance,
                                   sin(theta)*sin(phi)*cameraDistance));
        camera->setTarget(cameraTarget->getPosition());
    }
    reg<Player> player = mayor.getPlayer();
    if(!inventory && player.exists()) {
        inventory = guienv->addWindow(getBottomRightRect(192+8,300));
        new GUIInventory(inventory,player->inventory());
    }
    driver->beginScene(true,true,SColor(255,100,101,140));
    smgr->drawAll();
    guienv->drawAll();
    driver->endScene();
    driver->setMaterial(material);
}

bool Render::run()
{
    return device->run();
}

void Render::moveMouse(int x,int y,bool left,bool right)
{
    Position pos = getPositionUnderMouse(x,y);
    if(!(pos==lastMousePosition))
        getLocatablesUnderMouse(0,x,y);
    lastMousePosition = pos;
    lastMouseLocation = core::position2di(x,y);
}

void Render::updateOptionList() {
    if(optionListContextMenu) {
        optionListContextMenu->drop();
        optionListContextMenu = 0;
    }
    if(optionList.options.size()>0)
    {
        optionListContextMenu = guienv->addContextMenu(core::rect<s32>(lastMouseLocation,core::dimension2di(250,200)),0,-1);
        for(int i=0; i<optionList.options.size(); i++)
        {
            optionListContextMenu->addItem(core::stringw(optionList.options[i]->getDescription().c_str()).c_str());
        }
    }
}

void Render::mouseLeftClick(int x,int y) {
    optionList.options.clear();
    irr::gui::IGUIElement* element = guienv->getRootGUIElement()->getElementFromPoint(irr::core::position2d<s32>(x,y));
    if(element && element->getParent() && element->getID()>0) {
        switch(element->getID()) {
        case 1: //  Item
            GUIDataElement<Item>* itemData = static_cast<GUIDataElement<Item>*>(element);
            Item* item = itemData->getData();
            OptionList* ol = item->getOptionList(mayor.getPlayer().get());
            if(ol) optionList.addList(*ol);
            updateOptionList();
            break;
        }
    } else {
        getLocatablesUnderMouse(&leftClick,x,y);
        processLeftClick();
    }
}

void Render::mouseRightClick(int x,int y) {
    mayor.log << "Mouse right click" << std::endl;
    //getOptionListsUnderMouse(x,y);
    Position end = getPositionUnderMouse(x,y);
    reg<Player> player = mayor.getPlayer();
    if(!player.exists()) return;
    Position start = player->getPosition();
    if(end.isValid()) {
        core::line3d<f32> ray;
        ray.start = vector3df(start.x,getMapHeight(start.x,start.y)+0.1,start.y);
        float height = getMapHeight(end.x,end.y);
        ray.end = vector3df(end.x,height+0.1,end.y);
        core::vector3df intersection;
        core::triangle3df hitTriangle;
        if(!smgr->getSceneCollisionManager()->
            getSceneNodeAndCollisionPointFromRay(ray,
                                             intersection,
                                             hitTriangle,
                                             4)) {
            mayor.walkTo(end.x,end.y);
            reg<ISceneNode> node(player.id());
            //if(node.exists())
            //    walkTo(node.get(),end.x,height,end.y);
        }
    }
}

void Render::moveCamera(float x,float y,float z)
{
    cameraAngle += x;
    cameraHeight += y;
    cameraDistance += z;
    if(cameraHeight<1) cameraHeight = 1;
    if(cameraHeight>60) cameraHeight = 60;
    if(cameraDistance<1) cameraDistance = 1;
    if(cameraDistance>30) cameraDistance = 30;
}

irr::core::rect<s32> Render::getTopLeftRect(int w,int h) {
    return irr::core::rect<s32>(0,0,w,h);
}
irr::core::rect<s32> Render::getTopRightRect(int w,int h) {
    return irr::core::rect<s32>(width-w,0,width,h);
}
irr::core::rect<s32> Render::getBottomLeftRect(int w,int h) {
    return irr::core::rect<s32>(0,height-h,w,height);
}
irr::core::rect<s32> Render::getBottomRightRect(int w,int h) {
    return irr::core::rect<s32>(width-w,height-h,width,height);
}

Position Render::getPositionUnderMouse(int mx,int my) {
    mayor.log << "Get position under mouse" << std::endl;
    core::line3df ray =
        collision->getRayFromScreenCoordinates(core::position2di(mx,my),
                                               camera);
    core::vector3df intersection;
    core::triangle3df hitTriangle;
    if(currentMap.exists()) {
        if(collision->getSceneNodeAndCollisionPointFromRay(ray,intersection,hitTriangle,1))
        {
            float x = intersection.X;
            float y = intersection.Z;
            return Position(x,y,currentMap.get());
        }
    }
    return Position();
}

void Render::getLocatablesUnderMouse(std::vector<Locatable*>* locatables,int mx,int my) {
    mayor.log << "Get Locatables under mouse" << std::endl;
    if(locatables) locatables->clear();
    core::line3df ray =
        collision->getRayFromScreenCoordinates(core::position2di(mx,my),
                                               camera);
    core::vector3df intersection;
    core::triangle3df hitTriangle;
    while(ISceneNode* node = collision->getSceneNodeAndCollisionPointFromRay(ray,intersection,hitTriangle,2,entityParent))
    {
        reg<ISceneNode> sceneNode = library.getSceneNode(node);
        reg<Locatable> locatable = library.getLocatable(sceneNode.id());
        if(locatable.exists()) {
            if(locatables)
                locatables->push_back(locatable);
            mayor.getLocatableOptionList(locatable.id());
        }
        ray.start = intersection + ROUNDING_ERROR_f32 * (ray.end - ray.start);
    }
    if(currentMap.exists()) {
        if(collision->getSceneNodeAndCollisionPointFromRay(ray,intersection,hitTriangle,1))
        {
            int x = intersection.X;
            int y = intersection.Z;
            Position pos(x,y,currentMap.get());
            if(pos.isValid()) {
/*                Tile* tile = pos.getTile();
                if(locatables)
                    locatables->push_back(tile);
                mayor.getTileOptionList(*tile);*/
            }
        }
    }
}


void Render::loadMap(Map* newmap)
{
    mayor.log << "Load map" << std::endl;
    if(!newmap) return;
    currentMap = library.getMap(newmap);
    int mwidth = newmap->width, mheight = newmap->height;
    scene::IAnimatedMesh* mesh = smgr->getMesh(newmap->path.c_str());
    //scene::IAnimatedMesh* mesh = smgr->getMesh("media/tree_scene.3ds");
    if (mesh)
    {
        mayor.log << "Mesh loaded" << std::endl;
        map = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, 1, 1024);
        map->setMaterialFlag(video::EMF_LIGHTING, false);
        ITriangleSelector* selector = smgr->createOctreeTriangleSelector(
                mesh, map, 128);
        map->setTriangleSelector(selector);
        selector->drop();
        for(int x=0; x<mwidth+1; x++)
        {
            for(int y=0; y<mheight+1; y++)
            {
                currentMap->heightmap[x][y] = (x*x+y*y)/150.0;
            }
        }
        if(cameraTarget) mayor.log << "camera target exists" << std::endl;
        if(cameraTarget && library.isSceneNode(cameraTarget)) {
            mayor.log << "camera target is scene node" << std::endl;
            reg<irr::scene::ISceneNode> target = library.getSceneNode(cameraTarget);
            if(library.isLocatable(target.id())) {
                mayor.log << "camera target is locatable" << std::endl;
                reg<Locatable> locatable = library.getLocatable(target.id());
                mayor.log << (int)locatable->getPosition().map << "," << (int)currentMap.get() << std::endl;
                if(locatable->getPosition().map == currentMap.get()) {
                    mayor.log << "camera target is on this map" << std::endl;
                    float x = locatable->getPosition().x;
                    float y = locatable->getPosition().y;
                    moveSceneNode(locatable.get(),target.get(),x,y);
                    setMap(target.get(),map);
                }
            }
        }
        mayor.log << "after?" << std::endl;
    }
}

void Render::setMap(irr::scene::ISceneNode* node, irr::scene::ISceneNode* map) {
    mayor.log << "SET MAP" << std::endl;
    const core::aabbox3d<f32>& box = node->getBoundingBox();
    core::vector3df radius = box.MaxEdge - box.getCenter();
    scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
            map->getTriangleSelector(), node, radius);
    node->addAnimator(anim);
    anim->drop();

}

void Render::processOptionList(Locatable* locatable,OptionList* optionlist) {
    optionLists[locatable] = optionlist;
    typedef OptionList::Options::iterator Iterator;
    int index = 0;
    for(Iterator i = optionlist->options.begin();i != optionlist->options.end();++i,++index) {
        mayor.log << index << ": " << (*i)->getDescription();
    }

    if(std::find(leftClick.begin(),leftClick.end(),locatable) != leftClick.end()) processLeftClick();
    if(std::find(rightClick.begin(),rightClick.end(),locatable) != rightClick.end()) processRightClick();
}

void Render::processLeftClick() {
    typedef std::vector<Locatable*>::iterator Iterator;
    mayor.log << "elements=" << leftClick.size() << std::endl;
    for(Iterator i = leftClick.begin();i != leftClick.end();++i) {
        Locatable* locatable = *i;
        if(optionLists.count(locatable)>0) {
            mayor.log << "contains optionlist" << std::endl;
            OptionList* optionList = optionLists[locatable];
            if(optionList->options.size()>0) {
                mayor.log << "contains option" << std::endl;
                Player* player = mayor.getPlayer();
                optionList->options[0]->onChoose(player);
                leftClick.clear();
                rightClick.clear();
                return;
            }
        } else {
            return;
        }
    }
}

void Render::processRightClick() {
    optionList.options.clear();
    typedef std::vector<Locatable*>::iterator Iterator;
    mayor.log << "elements=" << leftClick.size() << std::endl;
    for(Iterator i = rightClick.begin();i != rightClick.end();++i) {
        Locatable* locatable = *i;
        if(optionLists.count(locatable)==0) {
            return;
        }
    }
    for(Iterator i = rightClick.begin();i != rightClick.end();++i) {
        Locatable* locatable = *i;
        //if(optionLists.count(locatable)>0) {
            mayor.log << "contains optionlist" << std::endl;
            OptionList* ol = optionLists[locatable];
            optionList.addList(*ol);

        //} else {
        //    return;
        //}
    }
    updateOptionList();
    leftClick.clear();
    rightClick.clear();

}

irr::scene::ISceneNode* Render::loadAnimatedMesh(const std::string& path, const std::string& texture) {
    mayor.log << path << std::endl;
    irr::scene::IAnimatedMeshSceneNode* mesh = smgr->addAnimatedMeshSceneNode(smgr->getMesh(path.c_str()),entityParent,2,core::vector3df(0,0,0));
    if(mesh)
    {
        mayor.log << "create tri selector" << std::endl;
        ITriangleSelector* selector = smgr->createTriangleSelectorFromBoundingBox(mesh);
        if(map) {
            mayor.log << "set map" << std::endl;
            setMap(mesh,map);
        }
        mesh->setMD2Animation(scene::EMAT_STAND);
        mesh->setMaterialTexture(0,getTexture(texture));
        mesh->setScale(vector3df(0.02,0.02,0.02));
        mesh->setMaterialFlag(video::EMF_LIGHTING,false);
        mesh->setTriangleSelector(selector);
        mayor.log << "done" << std::endl;
        selector->drop();
        return mesh;
    } else {
        return 0;
    }
}

irr::video::ITexture* Render::getTexture(const std::string& texture) {
    return driver->getTexture(texture.c_str());
}
void Render::setCameraTarget(ISceneNode* node) {
    mayor.log << "Set camera target" << std::endl;
    if(node) cameraTarget = node;
    getMapHeight(1,1);
}

void Render::setAnimation(ISceneNode* node,EMD2_ANIMATION_TYPE animation) {
    mayor.log << "Set animation" << std::endl;
    irr::scene::IAnimatedMeshSceneNode* mnode = (irr::scene::IAnimatedMeshSceneNode*)node;
    mnode->setMD2Animation(animation);
}

void Render::moveSceneNode(Locatable* locatable,irr::scene::ISceneNode* node,float x, float y) {
    mayor.log << "Move scene node" << std::endl;
    getMapHeight(x,y);
    irr::scene::IAnimatedMeshSceneNode* mnode = (irr::scene::IAnimatedMeshSceneNode*)node;
    mayor.log << "map height " << x << "," << y << " = " << getMapHeight(x,y) << std::endl;
    if(node->getPosition().X == 0 && node->getPosition().Z == 0)
        node->setPosition(vector3df(x,getMapHeight(x,y)+0.5,y));
    else if(locatable){
        Position currentPos(node->getPosition().X,node->getPosition().Z,locatable->getPosition().map);
        Position targetPos(x,y,locatable->getPosition().map);
        Move* move = new Move(locatable,currentPos,targetPos);
        mayor.getLocatablesInAction().insert(locatable);
        locatable->actions.clear();
        locatable->actions.add(move);
        mayor.log << (targetPos.x - currentPos.x) << "|" << (targetPos.y - currentPos.y);
        mnode->setMD2Animation(irr::scene::EMAT_RUN);
        vector3df myRotation;
        myRotation.Y = std::atan2(-move->getDY(),move->getDX()) * (180.0f / irr::core::PI);
        node->setRotation(myRotation);
        vector3df target = vector3df(x,getMapHeight(x,y)+0.5,y);
        mayor.log << "Target = " << target.X << "," << target.Y << "," << target.Z << std::endl;
        irr::scene::ISceneNodeAnimator* anim =
            node->getSceneManager()->createFlyStraightAnimator(node->getPosition(),
                    target,move->getDuration()*100);
        if(anim)
        {
            mayor.log << "animated !!!!!!!!!!" << std::endl;
            node->addAnimator(anim);
            //anim->drop();
            anim = 0;
        }
    }
}

float Render::getMapHeight(float x, float y) {
    core::line3d<f32> ray;
    ray.start = vector3df(x,3,y);
    ray.end = vector3df(x,-3,y);
    core::vector3df intersection;
    core::triangle3df hitTriangle;
    smgr->getSceneCollisionManager()->
        getSceneNodeAndCollisionPointFromRay(ray,
                                             intersection,
                                             hitTriangle,
                                             1);

    return (hitTriangle.pointA.Y + hitTriangle.pointB.Y + hitTriangle.pointC.Y) / 3;

}

/*
GUI* GUI::gui = 0;

GUI::GUI(int width_, int height_)
    : width(width_), height(height_),
    cameraHeight(5), cameraAngle(90),
    optionListActive(false), optionListContextMenu(0),
    player("Player"),chest("Chest"),openChest(0),openChestElement(0)
{
    guii = this;

    mayor.log << "Device initialization" << std::endl;
    device = createDevice(video::EDT_OPENGL, dimension2d<u32>(width,height),16,false,false,false,&eventReceiver);
    guienv = device->getGUIEnvironment();
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    collision = smgr->getSceneCollisionManager();
    mayor.log << "Camera initialization" << std::endl;
    camera = smgr->addCameraSceneNode(0,vector3df(0,4.5,0),vector3df(25,0,25));
    entityParent = smgr->addEmptySceneNode();

    mayor.log << "GUI initialization" << std::endl;

    /// GUI - Initialization

    eventReceiver.gui = guienv;
    device->setWindowCaption(L"Ennnovia - the ORPG");
    face.load("media/FreeMonoBold.ttf");
    fonts.resize(10);
    for(int i=0; i<10; i++)
    {
        fonts[i] = new CGUITTFont(driver);
        fonts[i]->attach(&face,12+i*2);
    }

    inventory = guienv->addWindow(core::rect<s32>(width-48*4-20,height-48*7-30,width,height),false,L"Inventory");
    inventory->setDraggable(false);
    inventory->getCloseButton()->setVisible(false);

    timer() = device->getTimer();

    mayor.log << "Map initialization" << std::endl;

    currentMap = new Map(50,50);
    currentMap->getTile(1,0).type = TileType::Rock();
    currentMap->getTile(1,1).type = TileType::Rock();
    currentMap->getTile(1,2).type = TileType::Rock();
    currentMap->getTile(2,2).type = TileType::Rock();
    currentMap->getTile(3,2).type = TileType::Rock();
    currentMap->getTile(3,1).type = TileType::Rock();
    currentMap->getTile(3,0).type = TileType::Rock();
    for(int x=0; x<51; x++)
    {
        for(int y=0; y<51; y++)
        {
            currentMap->heightmap[x][y] = (x*x+y*y)/150.0;
        }
    }
    player.setPosition(Position(4,4,currentMap));
    chest.setPosition(Position(2,2,currentMap));
    Item& item = chest.getItemAt(0,0);
    item.type = lua.getItemType("Coins");
    item.amount = 117;

    mayor.log << "3D initialization" << std::endl;


    mapx = new MapSceneNode(smgr->getRootSceneNode(), smgr, currentMap, 333);
    ITriangleSelector* selector = smgr->createTriangleSelectorFromBoundingBox(mapx);
    mapx->setTriangleSelector(selector);
    selector->drop();
    playerx = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/faerie.md2"),entityParent,1,core::vector3df(4.5,1,4.45));
    if(playerx)
    {
        selector = smgr->createTriangleSelectorFromBoundingBox(playerx);
        playerx->setMD2Animation(scene::EMAT_STAND);
        playerx->setMaterialTexture(0,driver->getTexture("media/faerie2.bmp"));
        playerx->setScale(vector3df(0.03,0.03,0.03));
        mayor.log << playerx->getMesh()->getFrameCount() << std::endl;
        mayor.log << playerx->getMesh()->getMeshType() << "/" << EAMT_MD2 << std::endl;
        playerx->setMaterialFlag(video::EMF_LIGHTING,false);
        playerx->setTriangleSelector(selector);
        selector->drop();
    }
    addEntity(&player,playerx);

    chestx = smgr->addMeshSceneNode(smgr->getMesh("media/treasure_chest.obj"),entityParent,1,core::vector3df(2.5,0,2.5));
    if(chestx)
    {
        selector = smgr->createTriangleSelectorFromBoundingBox(chestx);
        chestx->setMaterialTexture(0,driver->getTexture("media/treasure_chest.jpg"));
        chestx->setMaterialFlag(video::EMF_LIGHTING,false);
        chestx->setScale(vector3df(0.5,0.5,0.5));
        chestx->setRotation(vector3df(0,180,0));
        chestx->setTriangleSelector(selector);
        selector->drop();
    }
    addEntity(&chest,chestx);
    mayor.log << "Register player: " << &player << "->" << playerx << std::endl;
}

GUI::~GUI()
{
    device->drop();
}

void GUI::run()
{
    while(device->run())
    {
        if(device->isWindowActive())
            onRun();
        else
            device->yield();
    }
}

void GUI::onRun()
{

    camera->setPosition(playerx->getPosition()+vector3df(cos(cameraAngle*PI/180)*6,cameraHeight,sin(cameraAngle*PI/180)*6));
    camera->setTarget(playerx->getPosition());
    if(eventReceiver.IsKeyDown(irr::KEY_LEFT))
    {
        cameraAngle -= 0.5;
    }
    if(eventReceiver.IsKeyDown(irr::KEY_RIGHT))
    {
        cameraAngle += 0.5;
    }
    if(eventReceiver.IsKeyDown(irr::KEY_UP))
    {
        cameraHeight += 0.06;
    }
    if(eventReceiver.IsKeyDown(irr::KEY_DOWN))
    {
        cameraHeight -= 0.06;
    }
    if(eventReceiver.IsKeyDown(irr::KEY_SPACE))
    {
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
    if(top && top->getParent())
    {
        // the gui element belongs to the chest?
        if(top->getParent()->getID() == DT_CHEST && top->getParent() != guienv->getRootGUIElement() && openChest)
        {
            const boost::tuple<Chest*,int,int>& data = static_cast<GUIDataElement<boost::tuple<Chest*,int,int> > *>(top->getParent())->getData();
            Item& item = data.get<0>()->getItemAt(data.get<1>(),data.get<2>());
            ChestItem citem(data.get<0>(),data.get<1>(),data.get<2>());
            optionList.addList(citem.getOptionList(&player));
        }
    }
    else     // if not, then get the 3d entities under the mouse cursor
    {
        core::line3df ray = collision->getRayFromScreenCoordinates(eventReceiver.mouseState.Position,camera);
        core::vector3df intersection;
        core::triangle3df hitTriangle;
        if(ISceneNode* node = collision->getSceneNodeAndCollisionPointFromRay(ray,intersection,hitTriangle,0,mapx))
        {
            s32 id = node->getID();
            int x = id % currentMap->width;
            int y = (id - x) / currentMap->width;
            optionList.addList(currentMap->getTile(x,y).getOptionList(&player));
        }
        if(ISceneNode* node = collision->getSceneNodeAndCollisionPointFromRay(ray,intersection,hitTriangle,0,entityParent))
        {
            //  ray.start = intersection + ROUNDING_ERROR_f32 * (ray.end - ray.start);
            Entity* entity = getNodesEntity(node);
            if(entity != 0)
            {
                optionList.addList(entity->getOptionList(&player));
            }
        }
    }
    //}

    if(leftMouseClick)
    {
        if(optionListActive && currentOptionList.options.size()>0 && eventReceiver.contextMenuID != -1)
        {
            mayor.log << "x" << std::endl;
            currentOptionList.options[eventReceiver.contextMenuID]->onChoose(&player);
            mayor.log << "x" << std::endl;
            currentOptionList.options.clear();
            eventReceiver.contextMenuID = -1;
            optionListActive = false;
        }
        else if(optionList.options.size()>0)
        {
            optionList.options[0]->onChoose(&player);
        }
    }
    if(rightMouseClick)
    {
        currentOptionList = optionList;
        if(optionList.options.size()>0)
        {
            optionListActive = true;
            optionListContextMenu = guienv->addContextMenu(core::rect<s32>(eventReceiver.mouseState.Position,core::dimension2di(250,200)),0,-1);
            for(int i=0; i<optionList.options.size(); i++)
            {
                optionListContextMenu->addItem(core::stringw(optionList.options[i]->getDescription().c_str()).c_str());
            }
        }
    }

    if(openChest)
    {

    }


    driver->beginScene(true,true,SColor(255,100,101,140));
    smgr->drawAll();
    guienv->drawAll();
    player.updateActionSchedule();
    driver->endScene();
}


void GUI::onGUIElementClosed(IGUIElement* elem)
{
    //if(elem == openChestElement) {
    //closeGUIChest();
    //}
}

void GUI::updateInventory()
{
}

/* void GUI::createGUIChest(Chest* chest) {
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
     mayor.log << "Close open chest" << std::endl;
     openChest = 0;
     openChestElement->remove();
 }

void GUI::addEntity(Entity* entity, ISceneNode* node)
{
    entityToNode[entity] = node;
    nodeToEntity[node] = entity;
}

Entity* GUI::getNodesEntity(ISceneNode* node)
{
    return nodeToEntity[node];
}

ISceneNode* GUI::getEntitySceneNode(Entity* entity)
{
    return entityToNode[entity];
}

void GUI::removeEntity(Entity* entity)
{
    entityToNode.erase(entityToNode.find(entity));
    nodeToEntity.erase(nodeToEntity.find(getEntitySceneNode(entity)));
}*/
}
