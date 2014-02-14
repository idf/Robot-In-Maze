#include "sensor.h"

Sensor::Sensor()
{ }

Sensor::Sensor(int x, int y, SENSORTYPE type) : 
	_sensorXLocationOffset(x), _sensorYLocationOffset(y), _type(type)
{ }

Sensor::~Sensor()
{ }

float Sensor::getSensorReading()
{
	#ifdef HARDWARE
	// actually get distance from sensor
#else
	// simulation
#endif
}