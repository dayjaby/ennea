#ifndef ENNOVIA_LAYER_LAYER_HPP
#define ENNOVIA_LAYER_LAYER_HPP

namespace Ennovia {

    class Layer {
    public:
        virtual bool addOptions(OptionList& ol) = 0;
    };
}

#endif
