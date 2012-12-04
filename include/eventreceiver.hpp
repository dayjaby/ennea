/******************************************************************************
* CIrrEventReceiver
* =================
*
* CIrrEventReceiver has no restrictions. Credit would be appreciated, but not
* required.
* From: http://irrlicht.sourceforge.net/forum/viewtopic.php?t=24508
******************************************************************************/

#ifndef __CIRREVENTRECEIVER_H__
#define __CIRREVENTRECEIVER_H__

#include <irrlicht/irrlicht.h>
#include <iostream>
#include <sstream>


namespace Ennovia {
    using namespace irr;
    using namespace core;
    using namespace gui;
    using namespace io;
    using namespace std;



    struct MouseState
    {
        enum { Left = 0, Middle, Right};
        core::position2di Position;
        void setButtonDown(int i, bool b) {
            if(buttonDown_[i] && !b) buttonClick[i] = true;
            buttonDown_[i] = b;
        }
        bool buttonDown(int i) {
            return buttonDown_[i];
        }
        bool buttonClicked(int i) {
            bool st = buttonClick[i];
            buttonClick[i] = false;
            return st;
        }
        MouseState() {
            buttonDown_[0] = buttonDown_[1] = buttonDown_[2] = false;
            buttonClick[0] = buttonClick[1] = buttonClick[2] = false;
        }
    private:
        bool buttonDown_[3];
        bool buttonClick[3];
    };


    class MyEventReceiver : public IEventReceiver {
    public:
        virtual bool OnEvent(const SEvent& event);
        virtual bool IsKeyDown(EKEY_CODE keyCode) const {
            return KeyIsDown[keyCode];
        }
        IGUIEnvironment* gui;
        MouseState mouseState;
        MyEventReceiver();
        int contextMenuID;
        IGUIElement* hovered;
    private:
        bool KeyIsDown[KEY_KEY_CODES_COUNT];
    };


}

#endif /* __CIRREVENTRECEIVER_HEADER__ */
