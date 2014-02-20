#ifndef ARENA_H
#define ARENA_H

#define ARENA_X_SIZE 20
#define ARENA_Y_SIZE 15
#define ARENA_UNIT_DISTANCE 10

class Arena
{
public:
	enum GRIDTYPE{UNOCCUPIED = 0, OBSTACLE = 1, START = 2, GOAL = 3, UNEXPLORED = 4};
	Arena();
	~Arena();

	inline GRIDTYPE getGridType(int posX, int posY) { return _grid[posX][posY]; }
	inline void setGridType(int posX, int posY, GRIDTYPE gridType) { _grid[posX][posY] = gridType; }

	void init();
	bool isExploredFully();
private:
	GRIDTYPE _grid[ARENA_X_SIZE][ARENA_Y_SIZE];
};

#endif