#ifndef ENNOVIA_GRAPHICS_HPP
#define ENNOVIA_GRAPHICS_HPP

#include "misc/irrlicht.hpp"
#include <map>
#include <vector>
#include <string>

namespace Ennovia {

    using namespace irr;
    using namespace core;
    using namespace scene;
    using namespace video;
    using namespace gui;

    class Map;
    template <class T>
    class GUIDataElement : public gui::IGUIElement {
    public:
        GUIDataElement (T* data_, core::rect<s32> rect, IGUIEnvironment *environment, IGUIElement *parent)
            : IGUIElement(EGUIET_ELEMENT,environment,parent,0,rect), data(data_) {}
        T* getData() { return data; }
        void setData(const T* data_) { data = data_; }
    private:
        T* data;
    };
}


#endif
