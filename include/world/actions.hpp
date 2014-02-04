#ifndef ENNOVIA_ACTIONS_HPP
#define ENNOVIA_ACTIONS_HPP

#include <set>
#include "core.hpp"
#include "world/position.hpp"

namespace Ennovia {

    class Locatable;

    class Action
    {
    public:
        Action(Locatable* performer_) : performer(performer_),ticks(0),duration(1) {}
        virtual ~Action() {}
        int getDuration() { return duration; }
        int getTicks() { return ticks; }
        Locatable* getPerformer() {return performer; }
        void tick() {
            onTick();
            ticks++;
        }
        virtual void onTick();
        virtual void onFinish() {}
    protected:
        Locatable* performer;
        int ticks,duration;
    };

    class ActionList
    {
    public:
        void tick();
        ActionList& add(Action* action);

        size_t size() { return actions.size(); }
        void clear() { actions.clear(); }

        typedef std::set<boost::shared_ptr<Action> > Actions;
        Actions actions;

    };



    class Move : public Action {
    public:
        Move(Locatable* performer,const Position& start,const Position& end);
        virtual void onTick();
        virtual void onFinish();

        float getDistance() { return distance; }
        float getDX() { return dX; }
        float getDY() { return dY; }
    private:
        float distance;
        float dX,dY;
    };

}

#endif
