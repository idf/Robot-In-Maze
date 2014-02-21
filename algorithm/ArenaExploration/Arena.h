#ifndef ARENA_H
#define ARENA_H

#define ARENA_X_SIZE 20
#define ARENA_Y_SIZE 15
#define ARENA_UNIT_DISTANCE 10

enum GRIDTYPE{UNOCCUPIED = 0, OBSTACLE = 1, START = 2, GOAL = 3, UNEXPLORED = 4};

struct Grid
{
	GRIDTYPE type;
	bool isOnPath;
	bool isVisited;
};

class Arena
{
public:
	Arena();
	~Arena();

	inline GRIDTYPE getGridType(int posX, int posY) { return _grid[posX][posY].type; }
	inline void setGridType(int posX, int posY, GRIDTYPE gridType) { _grid[posX][posY].type = gridType; }

	inline bool getIsOnPath(int posX, int posY) { return _grid[posX][posY].type; }
	inline void setIsOnPath(int posX, int posY, bool value) { _grid[posX][posY].isOnPath = value; }

	void init();
	bool isExploredFully();
	GRIDTYPE getRealGridType(int posX, int posY);

private:
	Grid _grid[ARENA_X_SIZE][ARENA_Y_SIZE];
};


#endif