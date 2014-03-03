#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define ROBOT_SIZE 2

#include <vector>
#include <gtkmm.h>
#include "Arena.h"
#include "displayItem.h"
#include "robot.h"
#include "MapIO.h"

class MainWindow : public Gtk::Window
{
public:
	MainWindow();
	virtual ~MainWindow();
	void refreshDisplay();
	void start();
	Gtk::Button startButton;
protected:
	void testThread();
	Gtk::Table arenaDisplay;
	DisplayItem robotDisplay;
	Arena* arena;
	Robot* robot;
	Arena* fullArena;
	MapIO* io;
};

#endif