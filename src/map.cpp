#include "map.hpp"
#include "gui.hpp"

namespace Ennovia {

    TileType::TileType(std::string name_, std::string description_, std::string path_) :
    name(name_), description(description_), path(path_) {
    }

    std::string TileType::getName() {
        return name;
    }

    std::string TileType::onExamine(Tile* t) {
        return description;
    }

    OptionList TileType::getOptionList(Player* o, Tile* t) {
        return OptionList();
    }

    TileType* TileType::Grass() {
        static TileType type("Grass", "It is beautiful green grass.","media/grass1.png");
        type.canMove[0] = 1;
        type.canMove[1] = 1;
        type.canMove[2] = 1;
        type.canMove[3] = 1;
        return &type;
    }

    TileType* TileType::Rock() {
        static TileType type("Rock", "A grey rock.","media/rock.png");
        type.canMove[0] = 0;
        type.canMove[1] = 0;
        type.canMove[2] = 0;
        type.canMove[3] = 0;
        return &type;
    }


    Tile::Tile(TileType* type_) : type(type_) {}

    OptionList Tile::getOptionList(Player* o) {
        OptionList list = type->getOptionList(o,this);
        if(type->canMove[0] + type->canMove[1] + type->canMove[2] + type->canMove[3] > 0)
            list.add(new WalkTo(getPosition()));
        list.add(new Examine<Tile,TileType>(this,type));
        return list;
    }

    Map::Map(int width_, int height_) : width(width_), height(height_), tiles(width*height,TileType::Grass()), heightmap(width_+1,std::vector<float>(height_+1,0.0))
    {
        for(int x=0;x<width;x++) {
            for(int y=0;y<height;y++) {
                getTile(x,y).setPosition(Position(x,y,this));
            }
        }
    }

    float Map::getHeight(int x, int y) {
        return 0.25*(heightmap[x][y]+heightmap[x+1][y]+heightmap[x][y+1]+heightmap[x+1][y+1]);
    }

    const Tile& Map::getTile(int x, int y) const {
        return tiles[y*width+x];
    }
    Tile& Map::getTile(int x, int y) {
        return tiles[y*width+x];
    }

    bool Map::outOfRange(int x, int y) {
        return x<0 || y<0 || x>=width || y>= height;
    }

    bool Map::canGo(int x, int y, Direction direction) {
        Position current(x,y,this);
        Position next = current.move(direction);
        if(outOfRange(x,y),outOfRange(next.x,next.y)) {
            return false;
        } else if(isDirectionDiagonal(direction)) {
            Direction leftDir = rotateLeft45(direction);
            Direction rightDir = rotateRight45(direction);
            Position left = current.move(leftDir);
            Position right = current.move(rightDir);
            return canGo(x,y,leftDir) && canGo(left.x,left.y,rightDir) && canGo(x,y,rightDir) && canGo(right.x,right.y,leftDir);
        } else {
            Direction mirrored_direction = mirrorDirection(direction);
            const Tile& currentTile = getTile(x,y);
            const Tile& nextTile = getTile(next.x,next.y);
            return currentTile.type->canMove[(int)direction/2] && nextTile.type->canMove[(int)mirrored_direction/2];
        }
    }


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


    float PositionX::GoalDistanceEstimate(PositionX& goal) {

            return (goal.x - x) * (goal.x - x) + (goal.y - y) * (goal.y - y);
    }

    bool PositionX::IsGoal(PositionX& goal) {
            TileType* type = map->getTile(x,y).type;
            std::cout << *this << " to " << goal << " = " <<  (goal.x - x) * (goal.x - x) + (goal.y - y) * (goal.y - y) ;
            std::cout << GoalDistanceEstimate(goal);
            std::cout << (type->canMove[0] + type->canMove[1] + type->canMove[2] + type->canMove[3] > 0) << std::endl;
            return GoalDistanceEstimate(goal) <= goal.range * goal.range && (type->canMove[0] + type->canMove[1] + type->canMove[2] + type->canMove[3] > 0);
    }

    bool PositionX::GetSuccessors(AStarSearch<PositionX>* astar, PositionX* parent) {
            if(!map) return true;
            for(int dir = 0; dir < Direction::DIRECTIONS; dir++) {
                if(map->canGo(x,y,(Direction)dir)) {
                    PositionX next = move((Direction)dir);
                    if(!parent || !(next==*parent)) {
                        bool b = astar->AddSuccessor(next);
                        if(!b) return false;
                    }
                }
            }
            return true;
    }

    float PositionX::GetCost(PositionX& succ) {
            return GoalDistanceEstimate(succ);
    }

    bool PositionX::IsSameState(PositionX& other) {
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
        do {
            SearchState = astar.SearchStep();
        } while(SearchState == AStarSearch<PositionX>::SEARCH_STATE_SEARCHING);
        if(SearchState == AStarSearch<PositionX>::SEARCH_STATE_SUCCEEDED) {
            astar.GetSolutionStart();
        }
    }

    Pathfinding::~Pathfinding() {
        if(SearchState == AStarSearch<PositionX>::SEARCH_STATE_SUCCEEDED) {
            astar.FreeSolutionNodes();
        }
        astar.EnsureMemoryFreed();
    }

    Position* Pathfinding::next() {
        if(SearchState != AStarSearch<PositionX>::SEARCH_STATE_SUCCEEDED)
            return 0;
        PositionX* nextpos = astar.GetSolutionNext();
        if(nextpos)
        std::cout << "Solution pos: " << *nextpos << std::endl;
    //    if(nextpos->getSquaredDistance(target) <= range*range)
            //return 0;
        return nextpos;
    }


    int Move::getTicks() {
        return 4;
    }


    void Move::onPerform(Entity* performer) {
        Position newP = performer->getPosition().move(direction);
        const Position& oldP = performer->getPosition();
        if(performer) {
            irr::scene::IAnimatedMeshSceneNode* sceneNode = (irr::scene::IAnimatedMeshSceneNode*) GUI::get()->getEntitySceneNode(performer);
            sceneNode->setMD2Animation(irr::scene::EMAT_RUN);
            sceneNode->setRotation(irr::core::vector3df(0,(int)direction*45+90,0));
            irr::scene::ISceneNodeAnimator* anim =
                    sceneNode->getSceneManager()->createFlyStraightAnimator(sceneNode->getPosition(),
                                                                            sceneNode->getPosition()+irr::core::vector3df(newP.x-oldP.x,map->getHeight(newP.x,newP.y)-map->getHeight(oldP.x,oldP.y),newP.y-oldP.y),getTicks()*100);
            if(anim) {
                sceneNode->addAnimator(anim);
                anim->drop();
                anim = 0;
            }
        }
        performer->setPosition(newP);
    }

    void Move::onFinish(Entity* performer) {
        if(performer) {
            ((irr::scene::IAnimatedMeshSceneNode*) GUI::get()->getEntitySceneNode(performer))->setMD2Animation(irr::scene::EMAT_STAND);
        }
    }


    WalkTo::WalkTo(Position target_, float range_) : target(target_), range(range_) {}

    void WalkTo::onChoose(Player* o) {
        o->walkTo(target,range);
    }

    std::string WalkTo::getDescription() {
        std::stringstream ss;
        ss << "Go to " << target;
        return ss.str();
    }



}
