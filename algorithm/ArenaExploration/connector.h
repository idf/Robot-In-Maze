#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <map>
#include <ClientGame.h>

class Connector
{
public:
	Connector();
	~Connector();
	bool init();
	
	// sending data
	bool sendMovement(int dist);
	bool sendRotationClockwise(int deg);
	bool sendRotationCounterClockwise(int deg);
	std::map<int, int>* requestForSensorInformation();
	bool sendLocation();

private:
	ClientNetwork* network;
};

#endif