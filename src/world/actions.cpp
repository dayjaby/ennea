#include "world/actions.hpp"
#include "world/locatable.hpp"

namespace Ennovia {

    void Action::onTick() {}
    void ActionList::tick() {
        for(Actions::iterator i = actions.begin();i != actions.end();i++) {
            boost::shared_ptr<Action> action = *i;
            std::cout << action->getTicks() << ">=" << action->getDuration() << std::endl;
            if(action->getTicks()>=action->getDuration()) {
                action->onFinish();
                actions.erase(action);
            } else {
                action->tick();
                return;
            }
        }
    }

    ActionList& ActionList::add(Action* action) {
        actions.insert(boost::shared_ptr<Action>(action));
        return *this;
    }




}


