#ifndef ROBOT_H
#define ROBOT_H

#include "sensor.h"
#include "Arena.h"
#include "connector.h"

#include <iostream>
#include <vector>
#include <map>

#define IR_RANGE 40
#define IR_LONG_RANGE 50
#define US_RANGE 60

enum DIRECTION{DOWN = 0, LEFT = 90, UP = 180, RIGHT = 270};
DIRECTION& operator++(DIRECTION& orig);
DIRECTION& operator--(DIRECTION& orig);
DIRECTION& operator++(DIRECTION& orig, int);
DIRECTION& operator--(DIRECTION& orig, int);

class Robot
{
public:

	Robot(int x, int y, DIRECTION direction);
	~Robot();

	inline int getPosX() { return _posX; }
	inline int getPosY() { return _posY; }
	inline DIRECTION getDirection() { return _direction; }
	inline std::vector<Sensor*> getSensors() { return _sensors; };

	void rotateClockwise(int deg);
	void rotateCounterClockwise(int deg);
	void moveForward(int dist);
	std::map<Sensor*, int>* getDataFromSensor();

	void senseEnvironment(Arena* arena, Arena* fullArena);

#ifndef HARDWARE
	inline void setLocation(int x, int y) {_posX = x; _posY = y;}
#endif

private:
	int _posX, _posY;
	DIRECTION _direction;
	std::vector<Sensor*> _sensors;
	Connector* conn;
	void Robot::openHorizon(Arena* arena, int x, int y, DIRECTION direction, int range);
	void Robot::determineSensorLocation(int& sensorX, int& sensorY);
};

#endif