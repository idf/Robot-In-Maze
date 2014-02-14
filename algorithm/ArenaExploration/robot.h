#ifndef ROBOT_H

#include <iostream>
#include <list>
#include "sensor.h"

#define ROBOT_RADIUS 20

class Arena
{
public:
	
private:
	int _posX, _posY;
	int _direction;
	std::list<Sensor*> _sensors;
};

#endif