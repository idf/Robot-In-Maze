#include "connector.h"

#include <winsock2.h>
#include <Windows.h>
#include <map>

using namespace std;

Connector::Connector()
{ }

bool Connector::init() { return false; }
// sending data
bool Connector::sendMovement(){ return false; }
bool Connector::sendRotation(){ return false; }
map<int, float>* Connector::requestForSensorInformation(){ return NULL;}
bool Connector::sendLocation(){ return false; }

// receiving data
bool Connector::receiveData(){ return false; }