#include "connector.h"
#include <map>
#include <json/json.h>

using namespace std;

Connector::Connector()
{
	network = new ClientNetwork();
}
Connector::~Connector()
{ }

std::map<int, int>* Connector::parseSensorInfoWithAck()
{
	char buf[1000];
	while(NetworkServices::receiveMessage(network->ConnectSocket, buf, 1000) <= 0)
		;
	// ack
	char ack[1000];
	while(NetworkServices::receiveMessage(network->ConnectSocket, ack, 1000) <= 0)
		;
	Json::Value root;
	Json::Reader reader;
	reader.parse(buf, root);
	Json::Value sensorInfoList = root["sensors"];
	std::map<int,int>* mymap = new std::map<int,int>();

	for (int i = 0; i < 6; ++i)
	{
		int sensor1 = sensorInfoList[i]["sensor"].asInt();
		int value1 = sensorInfoList[i]["value"].asInt();
		mymap->insert ( std::pair<int,int>(sensor1,value1) );
	}
	return mymap;
}
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
	Json::Value value2( deg );

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
	Json::Value value2( deg );

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

	return parseSensorInfoWithAck();
}

map<int, int>* Connector::sendMovementAndSense(int dist)
{
	Json::Value root;
	Json::Value value1( 20 );
	Json::Value value2( dist );

	root["function"] = value1;
	root["parameter"] = value2;

	Json::StyledWriter writer;
	std::string outputConfig = writer.write(root);
	char* temp = (char*)outputConfig.c_str();
	NetworkServices::sendMessage(network->ConnectSocket, (char*)outputConfig.c_str(), strlen(temp));
	
	return parseSensorInfoWithAck();
}
map<int, int>* Connector::sendRotationClockwiseAndSense(int deg)
{
	Json::Value root;
	Json::Value value1( 22 );
	Json::Value value2( deg );

	root["function"] = value1;
	root["parameter"] = value2;

	Json::StyledWriter writer;
	std::string outputConfig = writer.write(root);
	char* temp = (char*)outputConfig.c_str();
	NetworkServices::sendMessage(network->ConnectSocket, (char*)outputConfig.c_str(), strlen(temp));
	return parseSensorInfoWithAck();
}
map<int, int>* Connector::sendRotationCounterClockwiseAndSense(int deg)
{
	Json::Value root;
	Json::Value value1( 21 );
	Json::Value value2( deg );

	root["function"] = value1;
	root["parameter"] = value2;

	Json::StyledWriter writer;
	std::string outputConfig = writer.write(root);
	char* temp = (char*)outputConfig.c_str();
	NetworkServices::sendMessage(network->ConnectSocket, (char*)outputConfig.c_str(), strlen(temp));
	return parseSensorInfoWithAck();
}

// Android communication
bool Connector::waitForAndroidExplore()
{
	char buf[1000];
	while(NetworkServices::receiveMessage(network->ConnectSocket, buf, 1000) <= 0)
		;
	cout << buf << endl;
	if(buf == "explore")
		return true;
	else 
		return false;
}
bool Connector::waitForAndroidRun()
{
	char buf[1000];
	while(NetworkServices::receiveMessage(network->ConnectSocket, buf, 1000) <= 0)
		;
	cout << buf << endl;
	if(buf == "run")
		return true;
	else 
		return false;
}
bool Connector::sendMapInformationToAndroid(std::string map, std::string location)
{
	Json::Value roots;
	Json::Value mapV( map.c_str() );
	Json::Value locationV( location.c_str() );

	roots["map"] = mapV;
	roots["location"] = locationV;

	Json::StyledWriter writer;
	std::string output = writer.write(roots);

	char* temp = (char*)output.c_str();
	NetworkServices::sendMessage(network->ConnectSocket, (char*)output.c_str(), strlen(temp));

	return true;
}
