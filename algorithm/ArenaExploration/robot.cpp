#include "robot.h"	
#include "sensor.h"

Robot::Robot(int x, int y, int direction): _posX(x), _posY(y), _direction(direction)
{
	// initialize sensor configuration
}

Robot::~Robot(){}

void Robot::rotateClockwise(int deg)
{
#ifdef HARDWARE
	// hardware control
#endif
	_direction = (_direction + deg) % 360;
}

void Robot::moveForward(int dist)
{
#ifdef HARDWARE
	// hardware control
#endif
	// update location
	switch(_direction)
	{
		case 0:  // down
			++_posY; break;
		case 90: // left
			--_posX; break;
		case 180: // up
			--_posY; break;
		case 270: // right
			++_posX; break;
	}
}

float Robot::getDataFromSensor(int sensorIndex)
{
	return getSensors()[sensorIndex]->getSensorReading();
}