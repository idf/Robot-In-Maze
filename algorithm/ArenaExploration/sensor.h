#ifndef SENSOR_H
#define SENSOR_H

#define USFRONT_ID 0
#define IRFRONTL_ID 1
#define IRFRONTR_ID 2
#define IRLEFT_ID 10
#define IRRIGHT_ID 11
#define USSIDE_ID 12

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