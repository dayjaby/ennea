#include "entity.hpp"

namespace Ennovia {

    irr::ITimer*& timer() {
        static irr::ITimer* myTimer = 0;
        return myTimer;
    }

    Entity::Entity() : actionEnd(0) {
    }

    size_t Entity::getActionScheduleSize() {
        return actions.size();
    }

    void Entity::clearActionSchedule() {
        actions.clear();
    }

    void Entity::addToActionSchedule(Action* action) {
        actions.push_back(boost::shared_ptr<Action>(action));
    }

    void Entity::updateActionSchedule() {
        unsigned current = timer()->getTime();
        if(current >= actionEnd) {
            if(currentAction)
                currentAction->onFinish(this);
            if(actions.size()>0) {
                currentAction = actions.front();
                currentAction->onPerform(this);
                actions.pop_front();
                actionEnd = current + currentAction->getTicks()*100;
            } else {
                currentAction.reset();
            }
        }
    }

    bool isDirectionDiagonal(Direction d) {
        return ((int)d)%2 == 1;
    }

    Direction rotateLeft45(Direction d) {
        return (Direction)(((int)d+1)%Direction::DIRECTIONS);
    }

    Direction rotateRight45(Direction d) {
        return (Direction)(((int)d-1)%Direction::DIRECTIONS);
    }


    Direction mirrorDirection(Direction d) {
        return (Direction)(((int)d+(int)SOUTH)%Direction::DIRECTIONS);
    }


    Position::Position()
    : x(0), y(0)
    {
    }

    Position::Position(int x_, int y_, Map* map_)
    : x(x_), y(y_), map(map_)
    {
    }

    Position Position::move(Direction dir) const {
        return Position(x + (dir == Direction::NORTHEAST || dir == Direction::EAST || dir == Direction::SOUTHEAST)
                          - (dir == Direction::NORTHWEST || dir == Direction::WEST || dir == Direction::SOUTHWEST),
                        y + (dir == Direction::SOUTHWEST || dir == Direction::SOUTH || dir == Direction::SOUTHEAST)
                          - (dir == Direction::NORTHWEST || dir == Direction::NORTH || dir == Direction::NORTHEAST), map);
    }

    int Position::getSquaredDistance(const Position& pos) const {
        return (pos.x-x)*(pos.x-x)+(pos.y-y)*(pos.y-y);
    }

    Direction Position::getDirection(const Position& pos) const {
        if(x > pos.x) {
            if(y < pos.y) {
                return Direction::SOUTHWEST;
            } else if(y > pos.y) {
                return Direction::NORTHWEST;
            } else {
                return Direction::WEST;
            }
        } else if(x < pos.x) {
            if(y < pos.y) {
                return Direction::SOUTHEAST;
            } else if(y > pos.y) {
                return Direction::NORTHEAST;
            } else {
                return Direction::EAST;
            }
        } else {
            if(y < pos.y) {
                return Direction::SOUTH;
            } else {
                return Direction::NORTH;
            }
        }
    }

    std::ostream& operator<<(std::ostream& o, const Position& p) {
        return o << "(" << p.x << "," << p.y << ")";
    }


}
