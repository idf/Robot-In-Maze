#include "Arena.h"
#include "robot.h"
#include "sensor.h"
#include "MapIO.h"
#include "connector.h"
#include "pathfinder.h"

#include <ctime>

#ifdef GUI
#include <gtkmm.h>
#include "mainwindow.h"
#endif

using namespace std;

// global connector
// bad pattern but convenient
Connector* conn = new Connector();

int main(int argc, char* argv[])
{
	Arena* arena = new Arena();
	Robot* robot = new Robot(ARENA_START_X, ARENA_START_Y, 0);
	Arena* fullArena = new Arena();  // simulation purpose
	MapIO* io = new MapIO(arena, fullArena);

#ifndef HARDWARE
	io->readMapFromFile("testmap.txt");
	cout << "Map successfully read." << endl;

#ifdef DEBUG
	io->printArena(fullArena);
#endif

#endif
	PathFinder* pathFinder = new PathFinder(robot, fullArena);
	// wait for start event
	//while (!startEventReceived())
	//	;
	//pathFinder->explore();
	//// wait for run event
	//while (!runEventReceived())
	//	;
	//vector<Grid*> result = pathFinder->findPathBetween(robot->getPosX(), robot->getPosY(), ARENA_END_X, ARENA_END_Y);

#ifdef GUI
	Gtk::Main kit(argc, argv);
	MainWindow window;
	window.refreshDisplay(robot, fullArena, result);
	Gtk::Main::run(window);
#endif

	getchar();
}