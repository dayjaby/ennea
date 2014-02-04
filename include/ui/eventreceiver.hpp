#ifndef __CIRREVENTRECEIVER_H__
#define __CIRREVENTRECEIVER_H__

#include "misc/irrlicht.hpp"
#include <iostream>
#include <sstream>


namespace Ennovia {
    using namespace irr;
    using namespace irr::core;
    using namespace irr::gui;
    using namespace irr::io;
    using namespace std;

    const float cameraXSpeed = 10;
    const float cameraYSpeed = 10;
    const float cameraZSpeed = 0.2;

    class EventReceiver : public IEventReceiver {
    public:
        virtual bool OnEvent(const SEvent& event);

        IGUIEnvironment* gui;
        EventReceiver();
        int contextMenuID;
        IGUIElement* hovered;


    };


}

#endif /* __CIRREVENTRECEIVER_HEADER__ */
