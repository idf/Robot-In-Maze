#ifndef SENSOR_H
#define SENSOR_H

class Sensor
{
public:
	enum SENSORTYPE {IR, US, MC};
	Sensor();
	Sensor(int x, int y, int direction, SENSORTYPE type);
	~Sensor();

	inline float getXLocationOffset() { return _sensorXLocationOffset; }
	inline float getYLocationOffset() { return _sensorYLocationOffset; }
	inline SENSORTYPE getSensorType() { return _type; }

	float getSensorReading();
private:
	float _sensorXLocationOffset;
	float _sensorYLocationOffset;
	SENSORTYPE _type;
	int direction;
};

#endif