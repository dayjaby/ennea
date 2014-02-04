#ifndef ENNOVIA_MAP_HPP
#define ENNOVIA_MAP_HPP

#include <boost/multi_array.hpp>
#include <vector>
#include <sstream>
#include "misc/astar/stlastar.h"
#include "world/entity.hpp"
#include "world/position.hpp"
#include "world/object.hpp"


namespace Ennovia
{

class Tile;
class Map
{
public:
    typedef boost::multi_array<float,2> Heightmap;

    Map(int id,const std::string& path_, const std::string& heightmappath_, int width_, int height_);
    ~Map();

    std::string path,heightmappath;
    int width, height;
    Heightmap heightmap;
    typedef std::set<Locatable*> Locatables;
    Locatables locatables;

    float getHeight(int x, int y);
};
/*
// Wrapper for A*
class PositionX : public Position
{
public:
    PositionX();
    PositionX(const Position& pos);
    PositionX(int x_, int y_, Map* map_);

    float GoalDistanceEstimate(PositionX& goal);
    bool IsGoal(PositionX& goal);
    bool GetSuccessors(AStarSearch<PositionX>* astar, PositionX* parent);
    float GetCost(PositionX& succ);
    bool IsSameState(PositionX& other);
    float range;
};

std::ostream& operator<<(std::ostream& o, const Position& p);

class Pathfinding
{
public:
    Pathfinding(Position start, Position target_, float range_);
    ~Pathfinding();
    Position* next();
private:
    AStarSearch<PositionX> astar;
    unsigned int SearchState;
    Position target;
    float range;
};

/*class Move : public Action
{
public:
    Move(Map* map_, int direction_) : map(map_), direction(direction_) {}
    virtual ~Move() {}
    virtual int getTicks();
    virtual void onPerform(Object* performer);
    virtual void onFinish(Object* performer);
    Map* map;
    int direction;
};

class WalkTo : public Option
{
public:
    WalkTo(Position target_, float range_ = 0.1);
    virtual std::string getDescription();
    virtual void onChoose(Player* player);
private:
    float range;
    Position target;
};*/
}

#endif
