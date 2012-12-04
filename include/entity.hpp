#ifndef ENNOVIA_ENTITY_HPP
#define ENNOVIA_ENTITY_HPP

#include <irrlicht/irrlicht.h>
#include "actions.hpp"

namespace Ennovia {
    class Player;
    class Map;

    irr::ITimer*& timer();

    enum Direction { NORTH, NORTHWEST, WEST, SOUTHWEST, SOUTH, SOUTHEAST, EAST, NORTHEAST, DIRECTIONS };
    bool isDirectionDiagonal(Direction d);
    Direction rotateLeft45(Direction d);
    Direction rotateRight45(Direction d);
    Direction mirrorDirection(Direction d);

    class Position {
    public:
            int x, y;
            Map* map;

            Position();
            Position(int x_, int y_, Map* map_);

            bool operator==(const Position& pos) {
                return (x==pos.x && y==pos.y && map==pos.map);
            }

            int getSquaredDistance(const Position& pos) const;
            Position move(Direction dir) const;
            Direction getDirection(const Position& pos) const;
    };


    class Entity {
    public:
        Entity();
        virtual OptionList getOptionList(Player* o) {
            return OptionList();
        }
        const Position& getPosition() const { return position; }
        void setPosition(const Position& newP) {
            position = newP;
        }

        size_t getActionScheduleSize();
        void clearActionSchedule();
        void addToActionSchedule(Action* action);
        void updateActionSchedule();

    protected:
        typedef std::deque<boost::shared_ptr<Action> > Actions;
        Actions actions;

        boost::shared_ptr<Action> currentAction;
        unsigned actionEnd;
        Position position;
    };
}

#endif
