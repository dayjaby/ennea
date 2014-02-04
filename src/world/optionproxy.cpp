#include "world/optionlist.hpp"
#include "mayor.hpp"

namespace Ennovia {
    OptionProxy::OptionProxy(const std::string& desc_, int id_, int index_)
        : desc(desc_), id(id_),index(index_) {}

    OptionProxy::OptionProxy(const OptionProxy& option)
        : desc(option.desc), index(option.index) {}

    std::string OptionProxy::getDescription() {
        return desc;
    }

    void OptionProxy::onChoose(LivingObject* lo) {
        Mayor::get().chooseOption(id,index);
    }

}
