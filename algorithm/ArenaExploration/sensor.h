#ifndef SENSOR_H
#define SENSOR_H

class Sensor
{
public:
	enum SENSORTYPE {IR, US, MC};
	Sensor();
	Sensor(int id, float x, float y, int direction, SENSORTYPE type);
	~Sensor();

	inline float getXLocationOffset() { return _sensorXLocationOffset; }
	inline float getYLocationOffset() { return _sensorYLocationOffset; }
	inline SENSORTYPE getSensorType() { return _type; }
	inline int getID() { return _id; }
	inline int getDirection() { return _direction; }

private:
	float _sensorXLocationOffset;
	float _sensorYLocationOffset;
	SENSORTYPE _type;
	int _direction;
	int _id;
};

#endif