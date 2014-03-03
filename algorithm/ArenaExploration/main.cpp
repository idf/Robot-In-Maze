#include <gtkmm.h>
#include "mainwindow.h"

using namespace std;

int main(int argc, char* argv[])
{
	Gtk::Main kit(argc, argv);
	MainWindow window;
	Gtk::Main::run(window);
}