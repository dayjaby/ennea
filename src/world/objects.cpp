

namespace Ennovia {





    /*void TakeItemOption::onChoose(Player* p) {
        std::cout << "take item" << std::endl;
        switch(amount) {
            case -1:
                item.amount = 0;
                break;
            case 0:

                break;
            default:
                if(item.amount >= amount) {
                    item.amount -= amount;
                }
        }
        if(item.amount <= 0) {
            item.type = 0;
        }
        GUI::get()->updateGUIChest();
    }

    void OpenChestAction::onPerform(Entity* performer) {
        GUI::get()->createGUIChest(chest);
    }


    std::string OpenChestOption::getDescription() {
        return std::string("Open ") + chest->getName();
    }

    void OpenChestOption::onChoose(Player* p) {
        p->walkTo(chest->getPosition(),chest->getAccessDistance());
        p->addToActionSchedule(new OpenChestAction(chest));
    }

    OptionList Chest::getOptionList(Player* p) {
        OptionList ol;
        ol.add(new OpenChestOption(this));
        return ol.addList(Object::getOptionList(p));
    }

    int Chest::getWidth() { return 3; }
    int Chest::getHeight() { return 3; }*/
}
