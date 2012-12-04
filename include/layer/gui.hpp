#ifndef ENNOVIA_LAYER_GUI_HPP
#define ENNOVIA_LAYER_GUI_HPP

#include "layer/layer.hpp"

namespace Ennovia {

    class GUI : public Layer {
    public:
        virtual bool addOptions(OptionList& ol) = 0;
    };
}

#endif

