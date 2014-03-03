#include "mainwindow.h"
#include "MapIO.h"
#include "pathfinder.h"
#include <iostream>

using namespace std;

MainWindow::MainWindow()
	:arena(ARENA_X_SIZE, ARENA_Y_SIZE, true), robot(0, 0), startButton("test")
{
	// init table
	arena.set_row_spacings(2);
	arena.set_col_spacings(2);
	this->add(arena);
	Glib::signal_timeout().connect( sigc::mem_fun(*this, &MainWindow::refreshDisplay),
          250 );
	this->show_all();
}

// Display the arena based on current information
// unoccupied: white
// obstacle: #2E231F
// robot: #263C8B
// path: #4E74A6
// unexplored: #BDBF78
// start and end: #BFA524
bool MainWindow::refreshDisplay(Robot* robot, Arena* arena, std::vector<Grid*> path)
{
	Gtk::Button bt("test");
	bt.clicked();
	Gdk::Color unoccupied("white"), obstacle("#2E231F"), robotColor("#263C8B"), pathColor("#4E74A6"), unexplored("#BDBF78"), startAndEnd("#BFA524");

	// remove the existing display
	Glib::ListHandle<Widget*> childList = this->arena.get_children();
	Glib::ListHandle<Widget*>::iterator it = childList.begin();
	while (it != childList.end()) 
	{
		this->arena.remove(**it);
		it++;
	}

	// display path
	for (std::vector<Grid*>::iterator i = path.begin(); i != path.end(); ++i)
	{
		DisplayItem* onePath = new DisplayItem((*i)->getX(), (*i)->getY());
		onePath->modify_bg(Gtk::StateType::STATE_NORMAL, pathColor);
		this->arena.attach(*onePath, onePath->x, onePath->x+1, onePath->y, onePath->y+1);
	}

	// display robot
	this->robot.x = robot->getPosX();
	this->robot.y = robot->getPosY();
	this->robot.modify_bg(Gtk::StateType::STATE_NORMAL, robotColor);
	this->arena.attach(this->robot, this->robot.x, this->robot.x+ROBOT_SIZE, this->robot.y, this->robot.y+ROBOT_SIZE);

	// display Arena
	for (int i = 0; i < ARENA_X_SIZE; ++i)
	{
		for (int j = 0; j < ARENA_Y_SIZE; ++j)
		{
			DisplayItem* oneGrid = new DisplayItem(i, j);
			this->arena.attach(*oneGrid, oneGrid->x, oneGrid->x+1,oneGrid->y, oneGrid->y+1);
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
	this->show_all();
	return true;
}

MainWindow::~MainWindow()
{

}