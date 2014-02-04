#include "ui/eventreceiver.hpp"
#include "ui/render.hpp"
#include "mayor.hpp"

namespace Ennovia
{
EventReceiver::EventReceiver()
    : contextMenuID(-1), hovered(0), gui(0)
{
}

bool EventReceiver::OnEvent(const SEvent& event)
{
    std::cout << "Receive event" << std::endl;
    Render& render = Mayor::get().getRender();
    switch(event.EventType)
    {
    case irr::EET_GUI_EVENT:
        switch(event.GUIEvent.EventType)
        {
        case irr::gui::EGET_MENU_ITEM_SELECTED:
            contextMenuID = ((IGUIContextMenu*)event.GUIEvent.Caller)->getSelectedItem();
            break;
        case irr::gui::EGET_ELEMENT_FOCUSED:
            break;
        case irr::gui::EGET_ELEMENT_FOCUS_LOST:
            break;
        case irr::gui::EGET_ELEMENT_CLOSED:
            //GUI::get()->onGUIElementClosed(event.GUIEvent.Caller);
            break;
        default:
            break;
        }
        break;
    case irr::EET_KEY_INPUT_EVENT:
        if(event.KeyInput.PressedDown)
        {
            switch(event.KeyInput.Key)
            {
            case KEY_LEFT: render.moveCamera(-cameraXSpeed,0,0); break;
            case KEY_RIGHT: render.moveCamera(+cameraXSpeed,0,0); break;
            case KEY_DOWN: render.moveCamera(0,+cameraYSpeed,0); break;
            case KEY_UP: render.moveCamera(0,-cameraYSpeed,0); break;
            default: break;
            }
        }
        break;
    case irr::EET_MOUSE_INPUT_EVENT:
        switch(event.MouseInput.Event)
        {
        case EMIE_LMOUSE_LEFT_UP:
            std::cout << "left mouse click" << std::endl;
            render.mouseLeftClick(event.MouseInput.X, event.MouseInput.Y);
//            mouseState.setButtonDown(MouseState::Left,false);
            break;
        case EMIE_RMOUSE_LEFT_UP:
            render.mouseRightClick(event.MouseInput.X, event.MouseInput.Y);
//            mouseState.setButtonDown(MouseState::Right,false);
            break;
        case EMIE_MOUSE_MOVED:
            render.moveMouse(event.MouseInput.X, event.MouseInput.Y, event.MouseInput.isLeftPressed(), event.MouseInput.isRightPressed());
            break;
        case EMIE_MOUSE_WHEEL:
            render.moveCamera(0,0,-event.MouseInput.Wheel*cameraZSpeed);
            break;
        default:
            // mouse wheel stuff..
            break;
        }
        return gui->postEventFromUser(event);
    default:
        return false;
    }
    return false;
}
}
