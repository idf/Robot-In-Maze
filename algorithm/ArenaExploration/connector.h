#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <map>

class Connector
{
public:
	Connector();
	~Connector();
	bool init();
	
	// sending data
	bool sendMovement();
	bool sendRotation();
	map<int, float>* requestForSensorInformation();
	bool sendLocation();

	// receiving data
	bool receiveData();
private:
};

#endif