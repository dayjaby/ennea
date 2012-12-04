class 'ChestModule' (Module)

function ChestModule:__init()
    Module.__init(self)
end

function ChestModule:activate(chest)
    if(self.state > 0) then self:deactivate()
    self.window = self:getEnvironment(rects32(200,200,chest:getWidth()*48+204,chest:getHeight()*48+222),false,stringToWChar(chest:getName()))
    self.window:getMinimizeButton():setVisible(false)
    self.chest = chest
    self:update()
    self.state = 1
end

function ChestModule:update()
    if(self.state == 0) then return
    for child in self.window:getChildren()
        if(child:getID()>=0) child:remove()
    end
    self.items = {}
    for x = 0, openChest:getWidth(), 1 do
        self.items[x] = {}
        for y = 0, openChest:getHeight(), 1 do
            if(self.chest:getItemAt(x,y).type) then
                self.items[x][y] = {
            end
        end
    end
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
end

if(chest == openChest) return;
        if(openChest) {
            closeGUIChest();
        }
        openChestElement = guienv->addWindow(core::rect<s32>(200,200,chest->getWidth()*48+204,chest->getHeight()*48+222),false,stringw(chest->getName().c_str()).c_str());
        openChestElement->getMinimizeButton()->setVisible(true);
        openChest = chest;
        updateGUIChest();
