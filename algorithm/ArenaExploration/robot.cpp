#include "robot.h"	
#include "sensor.h"
#include "Arena.h"

using namespace std;

Robot::Robot(int x, int y, int direction): _posX(x), _posY(y), _direction(direction)
{
	// initialize sensor configuration
	Sensor* IRFront1 = new Sensor(1, 0, 0, Sensor::IR);
	Sensor* IRFront2 = new Sensor(-1, 0, 0, Sensor::IR);
	Sensor* IRLeft = new Sensor(0, 1, 90, Sensor::IR);
	Sensor* IRRight = new Sensor(0, 1, 270, Sensor::IR);
	getSensors().push_back(IRFront1);
	getSensors().push_back(IRFront2);
	getSensors().push_back(IRLeft);
	getSensors().push_back(IRRight);
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

// collect sensor information and update the arena information
void Robot::senseEnvironment(Arena* arena)
{
#ifdef HARDWARE
	vector<Sensor*> sensors = this->getSensors();
	for (vector<Sensor*>::const_iterator iter = sensors.begin(); iter != sensors.end(); ++iter)
	{
		Sensor* current = *iter;
		float sensorReading = 0;
		sensorReading = current->getSensorReading();  // return distance or angle
		switch (current->getSensorType())
		{
			// distance sensor, falling through case
			case Sensor::IR:
			case Sensor::US:
				if (sensorReading > 0)
				{

				}
				break;
			// Digital compass
			case Sensor::MC:
				// left empty
				// judge against the current direction
				break;
		}
	}
#else
	// simulation, don't need sensor data
	// manually set map explored
	switch(this->getDirection())
	{
	case 0:
		// front
		arena->setGridType(getPosX(), getPosY()+2, arena->getRealGridType(getPosX(), getPosY()+2));
		arena->setGridType(getPosX()+1, getPosY()+2, arena->getRealGridType(getPosX()+1, getPosY()+2));
		// left
		arena->setGridType(getPosX()-1, getPosY()+1, arena->getRealGridType(getPosX()-1, getPosY()+1));
		// right
		arena->setGridType(getPosX()+2, getPosY()+1, arena->getRealGridType(getPosX()+2, getPosY()+1));
	case 90:
		// front
		arena->setGridType(getPosX()-1, getPosY(), arena->getRealGridType(getPosX()-1, getPosY()));
		arena->setGridType(getPosX()-1, getPosY()+1, arena->getRealGridType(getPosX()-1, getPosY()+1));
		// left
		arena->setGridType(getPosX(), getPosY()+2, arena->getRealGridType(getPosX(), getPosY()+2));
		// right
		arena->setGridType(getPosX(), getPosY()-1, arena->getRealGridType(getPosX(), getPosY()-1));
	case 180:

	case 270:
	}
#endif
}


// calculate the next move based on information and past path
void Robot::exploreNextStep(Arena* arena)
{

}