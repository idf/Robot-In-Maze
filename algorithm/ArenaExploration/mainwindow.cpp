#include "mainwindow.h"
#include "MapIO.h"
#include "pathfinder.h"
#include <iostream>

using namespace std;

MainWindow::MainWindow()
	:arenaDisplay(ARENA_X_SIZE, ARENA_Y_SIZE, true), robotDisplay(0, 0), startButton("test")
{
	arena = new Arena();
	robot = new Robot(ARENA_START_X, ARENA_START_Y, 0);
	fullArena = new Arena();  // simulation purpose
	io = new MapIO(arena, fullArena);
	
	// init table
	arenaDisplay.set_row_spacings(2);
	arenaDisplay.set_col_spacings(2);
	this->add(arenaDisplay);
	//startButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::refreshDisplay));
	Glib::signal_idle().connect_once( sigc::mem_fun(*this, &MainWindow::start));
	this->show_all();
}

void MainWindow::start()
{
	#ifndef HARDWARE
	io->readMapFromFile("testmap.txt");
	cout << "Map successfully read." << endl;
#endif
	PathFinder* pathFinder = new PathFinder(robot, arena, fullArena, this);
	pathFinder->explore();
}

// Display the arena based on current information
// unoccupied: white
// obstacle: #2E231F
// robot: #263C8B
// path: #4E74A6
// unexplored: #BDBF78
// start and end: #BFA524
void MainWindow::refreshDisplay()
{
	Gdk::Color unoccupied("white"), obstacle("#2E231F"), robotColor("#263C8B"), pathColor("#4E74A6"), unexplored("#BDBF78"), startAndEnd("#BFA524");

	// remove the existing display
	arenaDisplay.hide_all();
	Glib::ListHandle<Widget*> childList = this->arenaDisplay.get_children();
	Glib::ListHandle<Widget*>::iterator it = childList.begin();
	while (it != childList.end()) 
	{
		this->arenaDisplay.remove(**it);
		it++;
	}

	// display path
	//for (std::vector<Grid*>::iterator i = path.begin(); i != path.end(); ++i)
	//{
	//	DisplayItem* onePath = new DisplayItem((*i)->getX(), (*i)->getY());
	//	onePath->modify_bg(Gtk::StateType::STATE_NORMAL, pathColor);
	//	this->arenaDisplay.attach(*onePath, onePath->x, onePath->x+1, onePath->y, onePath->y+1);
	//}

	// display robot
	this->robotDisplay.x = robot->getPosX();
	this->robotDisplay.y = robot->getPosY();
	this->robotDisplay.modify_bg(Gtk::StateType::STATE_NORMAL, robotColor);
	this->arenaDisplay.attach(this->robotDisplay, this->robotDisplay.x, this->robotDisplay.x+ROBOT_SIZE, this->robotDisplay.y, this->robotDisplay.y+ROBOT_SIZE);

	// display Arena
	for (int i = 0; i < ARENA_X_SIZE; ++i)
	{
		for (int j = 0; j < ARENA_Y_SIZE; ++j)
		{
			DisplayItem* oneGrid = new DisplayItem(i, j);
			this->arenaDisplay.attach(*oneGrid, oneGrid->x, oneGrid->x+1,oneGrid->y, oneGrid->y+1);
			switch(arena->getGridType(i, j))
			{
			case UNOCCUPIED:
				oneGrid->modify_bg(Gtk::StateType::STATE_NORMAL, unoccupied);
				break;
			case OBSTACLE:
				oneGrid->modify_bg(Gtk::StateType::STATE_NORMAL, obstacle);
				break;
			case UNEXPLORED:
				oneGrid->modify_bg(Gtk::StateType::STATE_NORMAL, unexplored);
				break;
			case START:
			case GOAL:
				oneGrid->modify_bg(Gtk::StateType::STATE_NORMAL, startAndEnd);
				break;
			}
		}
	}
	arenaDisplay.show_all();
	//return true;
}

MainWindow::~MainWindow()
{  }