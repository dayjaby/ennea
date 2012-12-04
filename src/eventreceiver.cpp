#include "eventreceiver.hpp"
#include "gui.hpp"

namespace Ennovia {
        MyEventReceiver::MyEventReceiver()
     : contextMenuID(-1), hovered(0), gui(0) {
        for(int i=0;i<KEY_KEY_CODES_COUNT;++i) {
            KeyIsDown[i] = false;
        }
    }

    bool MyEventReceiver::OnEvent(const SEvent& event) {
        switch(event.EventType) {
            case irr::EET_GUI_EVENT:
                switch(event.GUIEvent.EventType) {
                    case irr::gui::EGET_MENU_ITEM_SELECTED:
                        contextMenuID = ((IGUIContextMenu*)event.GUIEvent.Caller)->getSelectedItem();
                        break;
                    case irr::gui::EGET_ELEMENT_FOCUSED:
                        mouseState.setButtonDown(MouseState::Left,false);
                        break;
                    case irr::gui::EGET_ELEMENT_FOCUS_LOST:
                        break;
                    case irr::gui::EGET_ELEMENT_CLOSED:
                        GUI::get()->onGUIElementClosed(event.GUIEvent.Caller);
                        break;
                    default:
                        break;
                }
                break;
            case irr::EET_KEY_INPUT_EVENT:
                KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
                break;
            case irr::EET_MOUSE_INPUT_EVENT:
                switch(event.MouseInput.Event) {
                    case EMIE_LMOUSE_PRESSED_DOWN:
                        mouseState.setButtonDown(MouseState::Left,true);
                        break;
                    case EMIE_MMOUSE_PRESSED_DOWN:
                        mouseState.setButtonDown(MouseState::Middle,true);
                        break;
                    case EMIE_RMOUSE_PRESSED_DOWN:
                        mouseState.setButtonDown(MouseState::Right,true);
                        break;
                    case EMIE_LMOUSE_LEFT_UP:
                        mouseState.setButtonDown(MouseState::Left,false);
                        break;
                    case EMIE_MMOUSE_LEFT_UP:
                        mouseState.setButtonDown(MouseState::Middle,false);
                        break;
                    case EMIE_RMOUSE_LEFT_UP:
                        mouseState.setButtonDown(MouseState::Right,false);
                        break;
                    case EMIE_MOUSE_MOVED:
                        mouseState.Position.X = event.MouseInput.X;
                        mouseState.Position.Y = event.MouseInput.Y;
                        break;
                    default:
                        // mouse wheel stuff..
                        break;
                }
                return gui->postEventFromUser(event);
                return false;
            default:
                return false;
        }
        return false;
    }
}
