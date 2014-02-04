#ifndef ENNOVIA_WINDOW_HPP
#define ENNOVIA_WINDOW_HPP

#include "misc/irrlicht.hpp"
namespace Ennovia {

    class Window {
    public:
        Window(irr::gui::IGUIWindow* window_) : window(window_) {
        }

        irr::gui::IGUIWindow* getGUIWindow() {
            return window;
        }

    private:
        irr::gui::IGUIWindow* window;
    };
}

#endif
