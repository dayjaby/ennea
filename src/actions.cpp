#include "actions.hpp"
#include "objects.hpp"

namespace Ennovia {

    int Action::getTicks() { return 10; }

    void Action::onPerform(Entity* performer) {}

    std::string Option::getDescription() { return "Default Action"; }

    void Option::onChoose(Player* player) {}

    OptionList& OptionList::add(Option* option) {
        options.push_back(boost::shared_ptr<Option>(option));
        return *this;
    }

    OptionList& OptionList::addList(const OptionList& other) {
        options.insert(options.end(),other.options.begin(),other.options.end());
        return *this;
    }

}


