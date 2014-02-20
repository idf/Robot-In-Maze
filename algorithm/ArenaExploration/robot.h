#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>
#include <vector>
#include "sensor.h"
#include "Arena.h"

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
	float getDataFromSensor(int sensorIndex);

	void senseEnvironment(Arena* arena);
	void exploreNextStep(Arena* arena);

private:
	int _posX, _posY;
	int _direction;
	std::vector<Sensor*> _sensors;
};

#endif