#include "sensor.h"

Sensor::Sensor()
{ }

Sensor::Sensor(int x, int y, int direction, SENSORTYPE type) : 
	_sensorXLocationOffset(x), _sensorYLocationOffset(y), direction(direction), _type(type)
{ }

Sensor::~Sensor()
{ }
