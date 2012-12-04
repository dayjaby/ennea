#ifndef ENNOVIA_MAP_HPP
#define ENNOVIA_MAP_HPP

#include <vector>
#include <sstream>
#include "astar/stlastar.h"
#include "entity.hpp"
#include "objects.hpp"

namespace Ennovia {

    class Tile;

    class TileType {
    public:
        TileType(std::string name_, std::string description_, std::string path_);

        virtual std::string getName();
        virtual std::string onExamine(Tile* t);
        virtual std::string getTexturePath() { return path; }
        virtual OptionList getOptionList(Player* o, Tile* t);
        static TileType* Grass();
        static TileType* Rock();
        bool canMove[4];
    private:
        std::string name, description, path;
    };


    class Tile : public Entity {
    public:
        Tile(TileType* type_);
        virtual OptionList getOptionList(Player* o);
    //private:
        TileType* type;
        int x,y;
    };


    class Map {
    public:

        Map(int width_, int height_);

        int width, height;
        std::vector<Tile> tiles;
        std::vector<std::vector<float> > heightmap;

        float getHeight(int x, int y);
        const Tile& getTile(int x, int y) const;
        Tile& getTile(int x, int y);

        bool outOfRange(int x, int y);
        bool canGo(int x, int y, Direction d);
    };



    // Wrapper for A*
    class PositionX : public Position {
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

    class Pathfinding {
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

    class Move : public Action {
    public:
        Move(Map* map_, Direction direction_) : map(map_), direction(direction_) {}
        virtual ~Move() {}
        virtual int getTicks();
        virtual void onPerform(Entity* performer);
        virtual void onFinish(Entity* performer);
        Map* map;
        Direction direction;
    };

    class WalkTo : public Option {
    public:
        WalkTo(Position target_, float range_ = 0.1);
        virtual std::string getDescription();
        virtual void onChoose(Player* player);
    private:
        float range;
        Position target;
    };

}

#endif
