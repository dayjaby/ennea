#ifndef ENNOVIA_PLUGINS_GUIPLUGIN_HPP
#define ENNOVIA_PLUGINS_GUIPLUGIN_HPP

namespace Ennovia {
    class GUIPlugin {
    public:
        GUIPlugin(IGUIElement* elem_) : elem(elem_) {
            elements[elem] = this;
        }

        virtual void open() = 0;
        virtual void update(Player& player) = 0;
        virtual void close() = 0;

        IGUIElement* getElement() { return elem; }

        static std::map<IGUIElement*,GUIPlugin*> elements;
    private:
        IGUIElement* elem;
    };
}

#endif

