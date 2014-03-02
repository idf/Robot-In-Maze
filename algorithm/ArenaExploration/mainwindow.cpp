#include "mainwindow.h"

MainWindow::MainWindow()
	:arena(ARENA_X_SIZE, ARENA_Y_SIZE, true),
	startArea(), goalArea(), robot()
{
	//this->set_default_size(600, 450);
	this->add(arena);
	arena.set_row_spacings(2);
	arena.set_col_spacings(2);
	
	// init robot display
	Gdk::Color* robotColor = new Gdk::Color("SkyBlue2");
	robot.modify_bg(Gtk::StateType::STATE_NORMAL, *robotColor);
	//arena.attach(robot, 0, 2, 0, 2);

	for(int i = 0; i < ARENA_X_SIZE; ++i)
		for(int j = 0; j < ARENA_Y_SIZE; ++j)
		{
			Gtk::EventBox* eb = new Gtk::EventBox();
			eb->set_size_request(30, 30);
			eb->modify_bg(Gtk::StateType::STATE_NORMAL, *robotColor);
			arena.attach(*eb, i, i+1, j, j+1);
		}
	arena.show_all_children();
	this->show_all_children();
}

MainWindow::~MainWindow()
{

}