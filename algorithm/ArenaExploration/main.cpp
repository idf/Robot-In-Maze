#include "Arena.h"
#include "robot.h"
#include "sensor.h"
#include "MapIO.h"
#include "connector.h"
#include "pathfinder.h"
#include <ctime>
#include <gtkmm.h>
#include "mainwindow.h"

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
#endif

	Gtk::Main kit(argc, argv);
	MainWindow window;
	Gtk::Main::run(window);
	
	PathFinder* pathFinder = new PathFinder(robot, arena, fullArena, &window);
	pathFinder->explore();

	// wait for start event
	//while (!startEventReceived())
	//	;
	//pathFinder->explore();
	//// wait for run event
	//while (!runEventReceived())
	//	;
}