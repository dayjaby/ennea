#include "world/position.hpp"
#include "world/map.hpp"

namespace Ennovia {

Position::Position()
    : x(0), y(0), map(0)
{
}

Position::Position(float x_, float y_, Map* map_)
    : x(x_), y(y_), map(map_)
{
}

Position Position::move(int dir) const
{
    return Position(x + (dir == NORTHEAST || dir == EAST || dir == SOUTHEAST)
                    - (dir == NORTHWEST || dir == WEST || dir == SOUTHWEST),
                    y + (dir == SOUTHWEST || dir == SOUTH || dir == SOUTHEAST)
                    - (dir == NORTHWEST || dir == NORTH || dir == NORTHEAST), map);
}

bool Position::isValid() const {
    return map && map->width>x && map->height>y;
}

float Position::getSquaredDistance(const Position& pos) const
{
    return (pos.x-x)*(pos.x-x)+(pos.y-y)*(pos.y-y);
}

int Position::getDirection(const Position& pos) const
{
    if(x > pos.x)
    {
        if(y < pos.y)
        {
            return SOUTHWEST;
        }
        else if(y > pos.y)
        {
            return NORTHWEST;
        }
        else
        {
            return WEST;
        }
    }
    else if(x < pos.x)
    {
        if(y < pos.y)
        {
            return SOUTHEAST;
        }
        else if(y > pos.y)
        {
            return NORTHEAST;
        }
        else
        {
            return EAST;
        }
    }
    else
    {
        if(y < pos.y)
        {
            return SOUTH;
        }
        else
        {
            return NORTH;
        }
    }
    return NORTH;
}

bool isDirectionDiagonal(int d)
{
    return ((int)d)%2 == 1;
}

int rotateLeft45(int d)
{
    return (int)(((int)d+1)%DIRECTIONS);
}

int rotateRight45(int d)
{
    return (int)(((int)d-1)%DIRECTIONS);
}


int mirrorDirection(int d)
{
    return (int)(((int)d+(int)SOUTH)%DIRECTIONS);
}

}
