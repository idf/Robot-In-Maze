#ifndef ARENAEXPLORER_H
#define ARENAEXPLORER_H

#include "Arena.h"
#include "robot.h"
#include "sensor.h"

class ArenaExplorer
{
public:
	ArenaExplorer();
	~ArenaExplorer();
	void ArenaExplorer::explore(Robot* robot, Arena* arena);
};

#endif