class Connector
{
public:
	Connector();
	~Connector();
	bool init();
	bool sendLocation();
	bool sendMovement();
	bool sendRotation();
	bool receiveData();
private:
};