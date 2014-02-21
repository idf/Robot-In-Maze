#include "sensor.h"

Sensor::Sensor()
{ }

Sensor::Sensor(int id, float x, float y, int direction, SENSORTYPE type) : 
	_id(id), _sensorXLocationOffset(x), _sensorYLocationOffset(y), _direction(direction), _type(type)
{ }

Sensor::~Sensor()
{ }
