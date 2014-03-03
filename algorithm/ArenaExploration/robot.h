#ifndef ROBOT_H
#define ROBOT_H

#include "sensor.h"
#include "Arena.h"

#include <iostream>
#include <vector>
#include <map>

#define ROBOT_RADIUS 20

class Robot
{
public:
	Robot(int x, int y, int direction);
	~Robot();

	inline int getPosX() { return _posX; }
	inline int getPosY() { return _posY; }
	inline int getDirection() { return _direction; }
	inline std::vector<Sensor*> getSensors() { return _sensors; };

	void rotateClockwise(int deg);
	void moveForward(int dist);
	std::map<Sensor*, float>* getDataFromSensor();

	void senseEnvironment(Arena* arena, Arena* fullArena);

#ifndef HARDWARE
	inline void setLocation(int x, int y) {_posX = x; _posY = y;}
#endif

private:
	int _posX, _posY;
	int _direction;
	std::vector<Sensor*> _sensors;
};

#endif