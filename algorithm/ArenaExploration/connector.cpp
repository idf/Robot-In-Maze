#include "connector.h"
#include <winsock2.h>
#include <Windows.h>
#include <map>
#include <json/json.h>

using namespace std;

Connector::Connector()
{
	network = new ClientNetwork();
}
Connector::~Connector()
{ }
bool Connector::init() { return false; }
// sending data
bool Connector::sendMovement(int dist)
{ 
	Json::Value root;
	Json::Value value1( 00 );
	Json::Value value2( dist );

	root["function"] = value1;
	root["parameter"] = value2;

	Json::StyledWriter writer;
	std::string outputConfig = writer.write(root);
	char* temp = (char*)outputConfig.c_str();
	NetworkServices::sendMessage(network->ConnectSocket, (char*)outputConfig.c_str(), strlen(temp));
	char buf[1000];
	while(NetworkServices::receiveMessage(network->ConnectSocket, buf, 1000) <= 0)
		;
	return true;
} 

bool Connector::sendRotationClockwise(int deg)
{ 
	Json::Value root;
	Json::Value value1( 02 );
	Json::Value value2( 90 );

	root["function"] = value1;
	root["parameter"] = value2;

	Json::StyledWriter writer;
	std::string outputConfig = writer.write(root);
	char* temp = (char*)outputConfig.c_str();
	NetworkServices::sendMessage(network->ConnectSocket, (char*)outputConfig.c_str(), strlen(temp));
	char buf[1000];
	while(NetworkServices::receiveMessage(network->ConnectSocket, buf, 1000) <= 0)
		;
	return true;
}

bool Connector::sendRotationCounterClockwise(int deg)
{ 
	Json::Value root;
	Json::Value value1( 01 );
	Json::Value value2( 90 );

	root["function"] = value1;
	root["parameter"] = value2;

	Json::StyledWriter writer;
	std::string outputConfig = writer.write(root);
	char* temp = (char*)outputConfig.c_str();
	NetworkServices::sendMessage(network->ConnectSocket, (char*)outputConfig.c_str(), strlen(temp));
	char buf[1000];
	while(NetworkServices::receiveMessage(network->ConnectSocket, buf, 1000) <= 0)
		;
	return true;
}
map<int, int>* Connector::requestForSensorInformation()
{
	Json::Value roots;
	Json::Value function( 10 );
	Json::Value parameter( 200 );

	roots["function"] = function;
	roots["parameter"] = parameter;

	Json::StyledWriter writer;
	std::string outputConfig = writer.write(roots);

	char* temp = (char*)outputConfig.c_str();
	NetworkServices::sendMessage(network->ConnectSocket, (char*)outputConfig.c_str(), strlen(temp));

	char buf[1000];
	while(NetworkServices::receiveMessage(network->ConnectSocket, buf, 1000) <= 0)
		;

	Json::Value root;
	Json::Reader reader;
	reader.parse(buf, root);

	int sensor1 = root["sensor"]["sensor_code1"].asInt();
	int value1 = root["sensor"]["return_value1"].asInt();
	
	int sensor2 = root["sensor"]["sensor_code2"].asInt();
	int value2 = root["sensor"]["return_value2"].asInt();

	int sensor3 = root["sensor"]["sensor_code3"].asInt();
	int value3 = root["sensor"]["return_value3"].asInt();

	int sensor4 = root["sensor"]["sensor_code4"].asInt();
	int value4 = root["sensor"]["return_value4"].asInt();

	int sensor5 = root["sensor"]["sensor_code5"].asInt();
	int value5 = root["sensor"]["return_value5"].asInt();

	std::map<int,int>* mymap = new std::map<int,int>();

	mymap->insert ( std::pair<int,int>(sensor1,value1) );
	mymap->insert ( std::pair<int,int>(sensor2,value2) );
	mymap->insert ( std::pair<int,int>(sensor3,value3) );
	mymap->insert ( std::pair<int,int>(sensor4,value4) );
	mymap->insert ( std::pair<int,int>(sensor5,value5) );

	return mymap;
}
bool Connector::sendLocation(){ return false; }
