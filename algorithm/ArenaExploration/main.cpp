#include "Arena.h"
#include "robot.h"
#include "sensor.h"

#include "pathfinder.h"

int main(int argc, char* argv[])
{
	Arena* arena = new Arena();
	Robot* robot = new Robot(0, 0, 0);
	PathFinder* pathFinder = new PathFinder(robot, arena);
	// wait for start event
	//while (!startEventReceived())
	//	;
	//pathFinder->explore();
	//// wait for run event
	//while (!runEventReceived())
	//	;
	pathFinder->findPathBetween(robot->getPosX(), robot->getPosY(), 18, 13);
}