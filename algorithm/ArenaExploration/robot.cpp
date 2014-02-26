#include "robot.h"	
#include "sensor.h"
#include "Arena.h"
#include "connector.h"

#include <map>

using namespace std;

Robot::Robot(int x, int y, int direction): _posX(x), _posY(y), _direction(direction)
{
	// initialize sensor configuration
	Sensor* IRFront1 = new Sensor(1, 1, 0, 0, Sensor::IR);
	Sensor* IRFront2 = new Sensor(2, -1, 0, 0, Sensor::IR);
	Sensor* IRLeft = new Sensor(3, 0, 1, 90, Sensor::IR);
	Sensor* IRRight = new Sensor(4, 0, 1, 270, Sensor::IR);
	getSensors().push_back(IRFront1);
	getSensors().push_back(IRFront2);
	getSensors().push_back(IRLeft);
	getSensors().push_back(IRRight);
}

Robot::~Robot(){}

void Robot::rotateClockwise(int deg)
{
#ifdef HARDWARE
	Connector* conn = new Connector();
	if (!conn->sendRotation())
		return;
#endif
	_direction = (_direction + deg) % 360;
}

void Robot::moveForward(int dist)
{
#ifdef HARDWARE
	// hardware control
	Connector* conn = new Connector();
	if (!conn->sendMovement())
		return;
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

map<Sensor*, float>* Robot::getDataFromSensor()
{
	map<Sensor*, float>* returnData = new map<Sensor*, float>();
	returnData->clear();
	Connector* conn = new Connector();
	map<int, float>* sensorData = conn->requestForSensorInformation();
	for (vector<Sensor*>::iterator iter = _sensors.begin(); iter != _sensors.end(); ++iter)
	{
		returnData->insert(pair<Sensor*, float>(*iter,  (*sensorData)[(*iter)->getID()]));
	}
	return returnData;
}

// collect sensor information and update the arena information
void Robot::senseEnvironment(Arena* arena)
{
#ifdef HARDWARE
	map<Sensor*, float>* sensorData = getDataFromSensor();
	for (map<Sensor*, float>::iterator iter = sensorData->begin(); iter != sensorData->end(); ++iter)
	{
		Sensor* current = iter->first;
		switch (current->getSensorType())
		{
		case Sensor::IR:
			break;
		case Sensor::US:
			break;
		case Sensor::MC:
			break;
		}
	}
#else
	// simulation, don't need sensor data
	// manually set map explored
	switch(this->getDirection())
	{
	case 0: // facing down
		// front
		arena->setGridType(getPosX(), getPosY()+2, arena->getRealGridType(getPosX(), getPosY()+2));
		arena->setGridType(getPosX()+1, getPosY()+2, arena->getRealGridType(getPosX()+1, getPosY()+2));
		// left
		arena->setGridType(getPosX()-1, getPosY()+1, arena->getRealGridType(getPosX()-1, getPosY()+1));
		// right
		arena->setGridType(getPosX()+2, getPosY()+1, arena->getRealGridType(getPosX()+2, getPosY()+1));
		break;
	case 90:  // left
		// front
		arena->setGridType(getPosX()-1, getPosY(), arena->getRealGridType(getPosX()-1, getPosY()));
		arena->setGridType(getPosX()-1, getPosY()+1, arena->getRealGridType(getPosX()-1, getPosY()+1));
		// left
		arena->setGridType(getPosX(), getPosY()+2, arena->getRealGridType(getPosX(), getPosY()+2));
		// right
		arena->setGridType(getPosX(), getPosY()-1, arena->getRealGridType(getPosX(), getPosY()-1));
		break;
	case 180: // facing up
		// front
		arena->setGridType(getPosX(), getPosY()-1, arena->getRealGridType(getPosX(), getPosY()-1));
		arena->setGridType(getPosX()+1, getPosY()-1, arena->getRealGridType(getPosX()+1, getPosY()-1));
		// left
		arena->setGridType(getPosX()-1, getPosY(), arena->getRealGridType(getPosX()-1, getPosY()));
		// right
		arena->setGridType(getPosX()+1, getPosY(), arena->getRealGridType(getPosX()+2, getPosY()));
		break;
	case 270:  // facing right
		// front
		arena->setGridType(getPosX()+2, getPosY(), arena->getRealGridType(getPosX()+2, getPosY()));
		arena->setGridType(getPosX()+2, getPosY()+1, arena->getRealGridType(getPosX()+2, getPosY()+1));
		// left
		arena->setGridType(getPosX()+1, getPosY()-1, arena->getRealGridType(getPosX()+1, getPosY()-1));
		// right
		arena->setGridType(getPosX()+1, getPosY()+2, arena->getRealGridType(getPosX()+1, getPosY()+2));
		break;
	}
#endif
}


// calculate the next move based on information and past path
// algorithm: AStar
// function: manhattan distance
void Robot::exploreNextStep(Arena* arena)
{
	
}