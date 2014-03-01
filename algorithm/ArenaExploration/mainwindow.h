#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define ROBOT_SIZE 2

#include <vector>
#include <gtkmm.h>
#include "Arena.h"
#include "displayItem.h"

class MainWindow : public Gtk::Window
{
public:
	MainWindow();
	virtual ~MainWindow();

protected:
	// singnal handlers
	Gtk::Table arena;
	Gtk::EventBox startArea, goalArea, robot;
	std::vector<DisplayItem> obstacles, path, exploredGrids;
};

#endif