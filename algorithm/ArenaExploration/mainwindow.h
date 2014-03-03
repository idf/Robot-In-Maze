#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define ROBOT_SIZE 2

#include <vector>
#include <gtkmm.h>
#include "Arena.h"
#include "displayItem.h"
#include "robot.h"

class MainWindow : public Gtk::Window
{
public:
	MainWindow();
	virtual ~MainWindow();
	void refreshDisplay(Robot* robot, Arena* arena, std::vector<Grid*> path);
protected:
	// singnal handlers


	Gtk::Table arena;
	DisplayItem robot;
	Gtk::HBox box;
	Gtk::Button startButton;
};

#endif