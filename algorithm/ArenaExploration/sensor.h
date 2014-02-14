#ifndef SENSOR_H

class Sensor
{
public:
	Sensor();
	~Sensor();
	enum SENSORTYPE {IR, US, MC};

private:
	int _sensorXLocationOffset;
	int _sensorYLocationOffset;
};

#endif