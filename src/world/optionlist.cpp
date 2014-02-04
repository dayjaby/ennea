#include "world/optionlist.hpp"

namespace Ennovia {


    std::string Option::getDescription() { return "Default Action"; }

    void Option::onChoose(LivingObject* lo) {}

    bool OptionList::isValid() {
        /** TODO: timestamp **/
        return true;
    }
    OptionList& OptionList::add(Option* option) {
        options.push_back(boost::shared_ptr<Option>(option));
        return *this;
    }

    OptionList& OptionList::addList(const OptionList& other) {
        options.insert(options.end(),other.options.begin(),other.options.end());
        return *this;
    }



}
