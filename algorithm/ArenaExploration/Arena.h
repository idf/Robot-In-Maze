#ifndef ARENA_H

#define ARENA_X_SIZE 20
#define ARENA_Y_SIZE 15

class Arena
{
public:
	enum GRIDTYPE{UNOCCUPIED, UNEXPLORED, OBSTACLE, START, GOAL};
	void init();
	void setGridType(int posX, int posY, GRIDTYPE gridType);
private:
	GRIDTYPE _grid[ARENA_X_SIZE][ARENA_Y_SIZE];
};

#endif