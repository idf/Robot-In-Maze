#include <gtkmm/main.h>
#include "mainwindow.h"

using namespace std;

int main(int argc, char* argv[])
{
	//Robot* r = new Robot(1, 1, DOWN);
	//Arena* arena = new Arena();
	//Arena* fullArena = new Arena();
	//MapIO* io = new MapIO(arena, fullArena);
	//r->senseEnvironment(arena, fullArena);
	//
	//io->printArena(arena);
	//getchar();

	Gtk::Main kit(argc, argv);
	MainWindow window;
	Gtk::Main::run(window);

}