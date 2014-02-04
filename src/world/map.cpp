#include "world/map.hpp"
//#include "ui/game.hpp"
#include "world/tile.hpp"
#include "world/player.hpp"
#include "misc/registry.hpp"

namespace Ennovia
{


Map::Map(int id,const std::string& path_, const std::string& heightmap_, int width_, int height_) :
path(path_), heightmappath(heightmap_), width(width_), height(height_), /*tiles(boost::extents[width_][height_]),*/ heightmap(boost::extents[width_+1][height_+1])
{
    std::cout << "Create map with id " << id << std::endl;
    reg<Entity> entity(id);
    if(entity.exists()) entity.erase();
    reg<Entity>(new Entity(),id);
    reg<Map>(this,id);
    reg<Map> mp(id);
    if(mp.get()==this) std::cout << "same map" << std::endl;
    for(int x=0; x<width; x++)
    {
        for(int y=0; y<height; y++)
        {
            //getTile(x,y) = Tile(Position(x,y,this), std::string(""));
        }
    }
}

Map::~Map() {
    std::cout << "destroy map" << std::endl;
}

float Map::getHeight(int x, int y)
{
    return 0.25*(heightmap[x][y]+heightmap[x+1][y]+heightmap[x][y+1]+heightmap[x+1][y+1]);
}


/*
PositionX::PositionX()
    : Position()
{
}

PositionX::PositionX(const Position& pos)
    : Position(pos.x, pos.y, pos.map)
{
}

PositionX::PositionX(int x_, int y_, Map* map_)
    : Position(x_,y_,map_)
{
}


float PositionX::GoalDistanceEstimate(PositionX& goal)
{

    return (goal.x - x) * (goal.x - x) + (goal.y - y) * (goal.y - y);
}

bool PositionX::IsGoal(PositionX& goal)
{
    return false;
    //Tile& tile = map->getTile(x,y);
    //return GoalDistanceEstimate(goal) <= goal.range * goal.range && (tile.canMove[0] + tile.canMove[1] + tile.canMove[2] + tile.canMove[3] > 0);
}

bool PositionX::GetSuccessors(AStarSearch<PositionX>* astar, PositionX* parent)
{
    if(!map) return true;
    for(int dir = 0; dir < DIRECTIONS; dir++)
    {
        if(map->canGo(x,y,(int)dir))
        {
            PositionX next = move((int)dir);
            if(!parent || !(next==*parent))
            {
                bool b = astar->AddSuccessor(next);
                if(!b) return false;
            }
        }
    }
    return true;
}

float PositionX::GetCost(PositionX& succ)
{
    return GoalDistanceEstimate(succ);
}

bool PositionX::IsSameState(PositionX& other)
{
    return x == other.x && y == other.y;
}

Pathfinding::Pathfinding(Position start, Position target_, float range_):
    astar(target_.map->width*target_.map->height), target(target_), range(range_)
{
    std::cout << "Range: "<< range << std::endl;
    PositionX startX(start);
    PositionX targetX(target);
    targetX.range = range;
    astar.SetStartAndGoalStates(startX,targetX);
    do
    {
        SearchState = astar.SearchStep();
    }
    while(SearchState == AStarSearch<PositionX>::SEARCH_STATE_SEARCHING);
    if(SearchState == AStarSearch<PositionX>::SEARCH_STATE_SUCCEEDED)
    {
        astar.GetSolutionStart();
    }
}

Pathfinding::~Pathfinding()
{
    if(SearchState == AStarSearch<PositionX>::SEARCH_STATE_SUCCEEDED)
    {
        astar.FreeSolutionNodes();
    }
    astar.EnsureMemoryFreed();
}

Position* Pathfinding::next()
{
    if(SearchState != AStarSearch<PositionX>::SEARCH_STATE_SUCCEEDED)
        return 0;
    PositionX* nextpos = astar.GetSolutionNext();
    return nextpos;
}

/*
int Move::getTicks()
{
    return 4;
}


void Move::onPerform(Locatable* performer)
{
    Position newP = performer->getPosition().move(direction);

    if(performer)
    {
        irr::scene::IAnimatedMeshSceneNode* sceneNode = (irr::scene::IAnimatedMeshSceneNode*) GUI::get()->getEntitySceneNode(performer);
        sceneNode->setMD2Animation(irr::scene::EMAT_RUN);
        sceneNode->setRotation(irr::core::vector3df(0,(int)direction*45+90,0));
        irr::scene::ISceneNodeAnimator* anim =
            sceneNode->getSceneManager()->createFlyStraightAnimator(sceneNode->getPosition(),
                    sceneNode->getPosition()+irr::core::vector3df(newP.x-oldP.x,map->getHeight(newP.x,newP.y)-map->getHeight(oldP.x,oldP.y),newP.y-oldP.y),getTicks()*100);
        if(anim)
        {
            sceneNode->addAnimator(anim);
            anim->drop();
            anim = 0;
        }
    }
    performer->setPosition(newP);
}

void Move::onFinish(Object* performer)
{
    if(performer)
    {
    //    ((irr::scene::IAnimatedMeshSceneNode*) GUI::get()->getEntitySceneNode(performer))->setMD2Animation(irr::scene::EMAT_STAND);
    }
}


WalkTo::WalkTo(Position target_, float range_) : target(target_), range(range_) {}

void WalkTo::onChoose(Player* o)
{
    o->walkTo(target,range);
}

std::string WalkTo::getDescription()
{
    std::stringstream ss;
    ss << "Go to " << "(" << target.x << "," << target.y << ")";
    return ss.str();
}
*/


}
