#ifndef ENNOVIA_ENTITY_HPP
#define ENNOVIA_ENTITY_HPP

namespace Ennovia {
    class LivingObject;
    class OptionList;
    class Entity {
    public:
        virtual ~Entity() {}
        virtual OptionList* getOptionList(LivingObject* o);

        /*virtual bool isLocatable() { return false; }
        virtual bool isLiving() { return false; }
        virtual bool isPlayer() { return false; }*/

    };


}

#endif
