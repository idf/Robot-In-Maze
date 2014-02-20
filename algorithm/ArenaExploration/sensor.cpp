#include "sensor.h"

Sensor::Sensor()
{ }

Sensor::Sensor(int x, int y, int direction, SENSORTYPE type) : 
	_sensorXLocationOffset(x), _sensorYLocationOffset(y), direction(direction), _type(type)
{ }

Sensor::~Sensor()
{ }

float Sensor::getSensorReading()
{
	switch (this->getSensorType())
	{
		// IR sensor
		case Sensor::IR:
			break;
		// Ultrasound sensor
		case Sensor::US:

			break;
		// Digital compass
		case Sensor::MC:

			break;
	}
}