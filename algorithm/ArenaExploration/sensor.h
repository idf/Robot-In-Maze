#ifndef SENSOR_H
#define SENSOR_H

#define IRFRONTL_ID 0
#define IRFRONTR_ID 1
#define IRFRONTC_ID 2
#define IRRIGHT_ID 3
#define USLEFT_ID 10
#define USRIGHT_ID 11

#define SMALL_IR_RANGE 30
#define US_RANGE 80

class Sensor
{
public:
	enum SENSORTYPE {IR, US, MC};
	Sensor();
	Sensor(int id, int direction, SENSORTYPE type);
	~Sensor();

	inline SENSORTYPE getSensorType() { return _type; }
	inline int getID() { return _id; }
	inline int getDirection() { return _direction; }

private:
	SENSORTYPE _type;
	int _direction;
	int _id;
};

#endif