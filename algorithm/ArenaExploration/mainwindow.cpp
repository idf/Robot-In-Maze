#include "mainwindow.h"

MainWindow::MainWindow()
	:arena(ARENA_X_SIZE, ARENA_Y_SIZE, true),
	startArea(), goalArea(), robot()
{
	this->set_default_size(600, 450);
	//arena.set_row_spacings(2);
	//arena.set_col_spacings(2);
	
	// init robot display
	Gdk::Color* robotColor = new Gdk::Color("SkyBlue2");
	robot.modify_bg(Gtk::StateType::STATE_NORMAL, *robotColor);
	Gtk::Label lab("LOL");
	Gtk::Button m_Button_1("LOLLL");
	arena.attach(m_Button_1, 3,4,3,4);



	arena.attach(lab, 0, 2, 0, 2);
	arena.show_all_children();
	this->show_all_children();
}

MainWindow::~MainWindow()
{

}