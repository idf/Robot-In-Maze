#include "sensor.h"

Sensor::Sensor()
{ }

Sensor::Sensor(int id, int direction, SENSORTYPE type) : 
	_id(id), _direction(direction), _type(type)
{ }

Sensor::~Sensor()
{ }
