#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define ROBOT_SIZE 2

#include <gtkmm.h>
#include "Arena.h"
#include "displayItem.h"
#include "robot.h"
#include "MapIO.h"
#include "pathfinder.h"

class MainWindow : public Gtk::Window
{
public:
	MainWindow();
	virtual ~MainWindow();
	void refreshDisplay();
	bool explore();
protected:
	Gtk::Table arenaDisplay;
	Arena* arena;
	Robot* robot;
	Arena* fullArena;
	MapIO* io;
	PathFinder* pathFinder;
	DisplayItem *gridDisplay[ARENA_X_SIZE][ARENA_Y_SIZE];
};

#endif