#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <map>
#include <ClientGame.h>
#include <string>

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
	bool Connector::waitForAndroidExplore();
	bool Connector::waitForAndroidRun();
	bool Connector::sendMapInformationToAndroid(std::string map, std::string location);
private:
	ClientNetwork* network;
};

#endif