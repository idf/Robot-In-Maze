#ifndef SENSOR_H
#define SENSOR_H

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