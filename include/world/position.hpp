#ifndef ENNOVIA_POSITION_HPP
#define ENNOVIA_POSITION_HPP

namespace Ennovia
{
static const int epsilon = 0.01;
enum Direction { NORTH, NORTHWEST, WEST, SOUTHWEST, SOUTH, SOUTHEAST, EAST, NORTHEAST, DIRECTIONS };
bool isDirectionDiagonal(int d);
int rotateLeft45(int d);
int rotateRight45(int d);
int mirrorDirection(int d);

class Map;
class Position
{
public:
    float x, y;
    Map* map;

    Position();
    Position(float x_, float y_, Map* map_);

    bool operator==(const Position& pos) const
    {
        return ((int)x == (int)pos.x && (int)y == (int)pos.y && map==pos.map);
    }

    float getSquaredDistance(const Position& pos) const;
    Position move(int dir) const;
    bool isValid() const;
    int getDirection(const Position& pos) const;
};


}
#endif
