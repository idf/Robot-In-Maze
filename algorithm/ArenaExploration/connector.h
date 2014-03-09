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
	std::map<int, int>* parseSensorInfoWithAck();

	// sending data
	bool sendMovement(int dist);
	bool sendRotationClockwise(int deg);
	bool sendRotationCounterClockwise(int deg);
	std::map<int, int>* requestForSensorInformation();

	std::map<int, int>* sendMovementAndSense(int dist);
	std::map<int, int>* sendRotationClockwiseAndSense(int deg);
	std::map<int, int>* sendRotationCounterClockwiseAndSense(int deg);

	bool Connector::waitForAndroidExplore();
	bool Connector::waitForAndroidRun();
	bool Connector::sendMapInformationToAndroid(std::string map, std::string location);
private:
	ClientNetwork* network;
};

#endif