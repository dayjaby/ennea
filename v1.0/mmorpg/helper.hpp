class Map
{
public:
	enum direction { NORTH, WEST, SOUTH, EAST, 
		NORTHEAST, NORTHWEST, SOUTHWEST, SOUTHEAST };
};

class Position {
public:
	float GoalDistanceEstimate(Position& goal);
	bool IsGoal(Position& goal);
	bool GetSuccessors(AStarSearch<Position>* astar, Position* parent);
	float GetCost(Position& succ);
	bool IsSameState(Position& other);
	int x, y;
	Map* map;

	Position();
	Position(int x_, int y_, Map* map_);
};

Position::Position()
: x(0), y(0)
{
}

Position::Position(int x_, int y_. Map* map_)
: x(x_), y(y_), map(map_)
{
}

float Position::GoalDistanceEstimate(Position& goal) {
	return ( goal.x - x ) * ( goal.x - x)
		+ ( goal.y - y ) * ( goal.y - y);
}

bool Position::IsGoal(Position& goal) {
	return IsSameState(goal);
}

bool Position::GetSuccessors(AStarSearch<Position>* astar, Position* parent) {
	if(!map) return;
	if(map->getTile(x,y).canGo(Tile::NORTH)) {
		if(!astar->AddSuccessor(Position(x,y-1))) return false;
	}
	if(map->getTile(x,y).canGo(Tile::WEST)) {
		if(!astar->AddSuccessor(Position(x-1,y))) return false;
	}
	if(map->getTile(x,y).canGo(Tile::SOUTH)) {
		if(!astar->AddSuccessor(Position(x,y+1))) return false;
	}
	if(map->getTile(x,y).canGo(Tile::EAST)) {
		if(!astar->AddSuccessor(Position(x+1,y))) return false;
	}
	if(map->getTile(x,y).canGo(Tile::NORTHEAST)) {
		if(!astar->AddSuccessor(Position(x+1,y-1))) return false;
	}
	if(map->getTile(x,y).canGo(Tile::NORTHWEST)) {
		if(!astar->AddSuccessor(Position(x-1,y-1))) return false;
	}
	if(map->getTile(x,y).canGo(Tile::SOUTHWEST)) {
		if(!astar->AddSuccessor(Position(x-1,y+1))) return false;
	}
	if(map->getTile(x,y).canGo(Tile::SOUTHEAST)) {
		if(!astar->AddSuccessor(Position(x+1,y+1))) return false;
	}
}

float Position::GetCost(Position& succ) {
	return GoalDistanceEstimate(succ);
}

bool Position::IsSameState(Position& other) {
	return x == other.x && y == other.y;
}

http://code.google.com/p/a-star-algorithm-implementation/downloads/detail?name=astar.zip&can=2&q=

