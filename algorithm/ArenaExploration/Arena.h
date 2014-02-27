#ifndef ARENA_H
#define ARENA_H

#define ARENA_X_SIZE 20
#define ARENA_Y_SIZE 15
#define ARENA_UNIT_DISTANCE 10

enum GRIDTYPE{UNOCCUPIED = 0, OBSTACLE = 1, START = 2, GOAL = 3, UNEXPLORED = 4};

class Grid
{
public:
	GRIDTYPE type;
	bool isOnPath;
	bool opened;
	bool closed;
	int distanceTravelled;
	int distanceEstimated;
	int x, y; // two way link coordination

	inline int getX() { return x; }
	inline int getY() { return y; }
};

class Arena
{
public:
	Arena();
	~Arena();

	inline Grid* getGrid(int posX, int posY) { return &_grid[posX][posY]; }

	inline GRIDTYPE getGridType(int posX, int posY) { return _grid[posX][posY].type; }
	inline void setGridType(int posX, int posY, GRIDTYPE gridType) { _grid[posX][posY].type = gridType; }

	inline bool getIsOnPath(int posX, int posY) { return _grid[posX][posY].isOnPath; }
	inline void setIsOnPath(int posX, int posY, bool value) { _grid[posX][posY].isOnPath = value; }

	inline bool getIsOpen(int posX, int posY) { return _grid[posX][posY].opened; }
	inline void setIsOpen(int posX, int posY, bool value) { _grid[posX][posY].opened = value; }

	inline bool getIsClosed(int posX, int posY) { return _grid[posX][posY].closed; }
	inline void setIsClosed(int posX, int posY, bool value) { _grid[posX][posY].closed = value; }

	inline int getDistanceTravelled(int posX, int posY) { return _grid[posX][posY].distanceTravelled; }
	inline void setDistanceTravelled(int posX, int posY, int value) { _grid[posX][posY].distanceTravelled = value; }

	inline int getDistanceEstimated(int posX, int posY) { return _grid[posX][posY].distanceEstimated; }
	inline void setDistanceEstimated(int posX, int posY, int value) { _grid[posX][posY].distanceEstimated = value; }

	void init();
	bool isExploredFully();
	GRIDTYPE getRealGridType(int posX, int posY);

private:
	Grid _grid[ARENA_X_SIZE][ARENA_Y_SIZE];
};
#endif