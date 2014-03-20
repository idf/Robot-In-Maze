#ifndef ROBOT_H
#define ROBOT_H

#include "sensor.h"
#include "Arena.h"
#include "connector.h"

#include <iostream>
#include <vector>
#include <map>

enum DIRECTION{DOWN = 0, LEFT = 90, UP = 180, RIGHT = 270};
DIRECTION& operator++(DIRECTION& orig);
DIRECTION& operator--(DIRECTION& orig);
DIRECTION& operator++(DIRECTION& orig, int);
DIRECTION& operator--(DIRECTION& orig, int);

class Robot
{
public:

	Robot(int x, int y, DIRECTION direction, Connector* conn);
	~Robot();

	inline int getPosX() { return _posX; }
	inline int getPosY() { return _posY; }
	inline DIRECTION getDirection() { return _direction; }

	void rotateClockwise(int deg);
	void rotateCounterClockwise(int deg);
	void moveForward(int dist);
	std::map<Sensor*, int>* getDataFromSensor();
	bool sendItselfAndArena(Arena* arena);
	std::map<Sensor*, int>* Robot::mapDataWithSensor(std::map<int, int>* data);

	void rotateClockwiseAndSense(int deg, Arena* arena);
	void rotateCounterClockwiseAndSense(int deg, Arena* arena);
	void moveForwardAndSense(int dist, Arena* arena);

	void calibrateAtGoal();
	void calibrateAtStart();
	void calibrateAtUpperRight();
	void calibrateAtBottomLeft();

	void openArenaWithSensorData(std::map<Sensor*, int>* sensorData, Arena* arena);
	void senseEnvironment(Arena* arena, Arena* fullArena);

	bool moveForwardWithDisplay(int dist);

#ifndef HARDWARE
	inline void setLocation(int x, int y) {_posX = x; _posY = y;}
#endif

private:
	int _posX, _posY;
	int movementCount;
	DIRECTION _direction;
	std::vector<Sensor*> _sensors;
	Connector* _conn;
	void Robot::openIRHorizon(Arena* arena, int x, int y, DIRECTION direction, int range);
	void Robot::openUSHorizon(Arena* arena, int x, int y, DIRECTION direction, int range);
};

#endif