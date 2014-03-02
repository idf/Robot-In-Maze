#include "Arena.h"
#include "robot.h"
#include "sensor.h"
#include "MapIO.h"
#include "connector.h"
#include "pathfinder.h"

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
	Arena* fullArena = new Arena();
	Robot* robot = new Robot(ARENA_START_X, ARENA_START_Y, 0);
	MapIO* io = new MapIO(arena, fullArena);
	io->readMapFromFile("testmap.txt");
	cout << "Map successfully read." << endl;

#ifdef DEBUG
	io->printArena(fullArena);
#endif

#ifdef GUI
	Gtk::Main kit(argc, argv);
	MainWindow window;
	Gtk::Main::run(window);

#endif

	PathFinder* pathFinder = new PathFinder(robot, fullArena);
	// wait for start event
	//while (!startEventReceived())
	//	;
	//pathFinder->explore();
	//// wait for run event
	//while (!runEventReceived())
	//	;
	vector<Grid*> result = pathFinder->findPathBetween(robot->getPosX(), robot->getPosY(), ARENA_END_X, ARENA_END_Y);

	getchar();
}