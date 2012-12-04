#include "plugins/GUIPlugin.hpp"

std::map<IGUIElement*,GUIPlugin*> GUIPlugin::elements = new std::map<IGUIElement*,GUIPlugin*>();
