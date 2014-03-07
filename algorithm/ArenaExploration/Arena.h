#ifndef ARENA_H
#define ARENA_H

#define ARENA_X_SIZE 20
#define ARENA_Y_SIZE 15
#define ARENA_UNIT_DISTANCE 10
#define ARENA_START_X 1
#define ARENA_START_Y 1
#define ARENA_END_X 18
#define ARENA_END_Y 13

#include <cstdlib>

enum GRIDTYPE{UNOCCUPIED = 0, OBSTACLE = 1, START = 2, GOAL = 3, UNEXPLORED = 4, UNSAFE = 5};

class Grid
{
public:
	GRIDTYPE type;
	bool opened;
	bool closed;
	int distanceTravelled;
	int distanceEstimated;
	int x, y, heuristic; // two way link coordination and heuristics
	Grid *parent;

	inline int getX() { return x; }
	inline int getY() { return y; }

	inline int Grid::getDistanceTravelled(Grid *p) { return p->distanceTravelled + ((x == p->x || y == p->y) ? 10 : 14); }
	int Grid::getDistanceEstimated(Grid *p);
	inline bool Grid::hasParent() { return parent != NULL; }
	void Grid::computeScores(Grid *end);
};

class Arena
{
public:
	Arena();
	~Arena();

	inline Grid* getGrid(int posX, int posY) { return &_grid[posX][posY]; }

	GRIDTYPE getGridType(int posX, int posY);
	void setGridType(int posX, int posY, GRIDTYPE gridType);

	void init();
	bool isExploredFully(int percentage);
	GRIDTYPE getRealGridType(int posX, int posY);

private:
	Grid _grid[ARENA_X_SIZE][ARENA_Y_SIZE];
};
#endif