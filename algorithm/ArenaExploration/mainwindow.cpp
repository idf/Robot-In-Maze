#include "mainwindow.h"
#include "MapIO.h"
#include "connector.h"
#include <iostream>


using namespace std;

MainWindow::MainWindow()
	:arenaDisplay(ARENA_X_SIZE, ARENA_Y_SIZE, false), startExploration("Explore")
{
	conn = new Connector();
	arena = new Arena();
	robot = new Robot(ARENA_START_X, ARENA_START_Y, RIGHT, conn);
	fullArena = new Arena();  // simulation purpose
	io = new MapIO(arena, fullArena);
	pathFinder = new PathFinder(robot, arena, fullArena, conn);
	
	// initialize display structure5
	percentageEntry.set_text("Percentage");
	timeLimitEntry.set_text("Time");
	this->add(vbox);
	vbox.pack_start(arenaDisplay);
	vbox.pack_start(hbox);
	hbox.pack_start(percentageEntry);
	hbox.pack_start(timeLimitEntry);
	hbox.pack_start(startExploration);

	// init table contant
	arenaDisplay.set_row_spacings(2);
	arenaDisplay.set_col_spacings(2);
	for (int i = 0; i < ARENA_X_SIZE; ++i)
	{
		for (int j = 0 ; j < ARENA_Y_SIZE; ++j)
		{
			gridDisplay[i][j] = new DisplayItem(i, j);
			gridDisplay[i][j]->modify_bg(Gtk::StateType::STATE_NORMAL, *(new Gdk::Color("#BDBF78")));
			this->arenaDisplay.attach(*gridDisplay[i][j], gridDisplay[i][j]->x, gridDisplay[i][j]->x+1,gridDisplay[i][j]->y, gridDisplay[i][j]->y+1);
		}
	}

#ifndef HARDWARE
	io->readMapFromFile("testmap.txt");
	cout << "Map successfully read." << endl;
#endif
	startExploration.signal_clicked().connect( sigc::mem_fun(*this,
              &MainWindow::startExplorationButtonClicked) );
	
	this->show_all();
}

void MainWindow::startExplorationButtonClicked()
{
	bool isExploreDone = false;
	conn->waitForAndroidExplore();
	pathFinder->start = time(0);
	robot->senseEnvironment(arena, fullArena);
	this->refreshDisplay();


	while(!isExploreDone)
	{
		isExploreDone = pathFinder->explore(atoi(percentageEntry.get_text().c_str()), atoi(timeLimitEntry.get_text().c_str()));
		this->refreshDisplay();
		io->printArena(arena);
		while(!robot->sendItselfAndArena(arena))
			;
	}
	
	robot->calibrateAtGoal();
	vector<Grid*> result = pathFinder->findPathBetween(robot->getPosX(), robot->getPosY(), ARENA_START_X, ARENA_START_Y, true);
	

	// display version
	bool isDone = false;
	vector<pair<std::string, int>*>* movementList = pathFinder->getMovementList(result);
	for (vector<pair<std::string, int>*>::iterator i = movementList->begin(); i != movementList->end(); ++i)
	{
		if((*i)->first == "rotateClockwise")
			robot->rotateClockwise((*i)->second);
		else if ((*i)->first == "rotateCounterClockwise")
			robot->rotateCounterClockwise((*i)->second);
		else
		{
			
			while(!isDone)
			{
				hideRobot(robot->getPosX(), robot->getPosY());
				isDone = robot->moveForwardWithDisplay((*i)->second);
				displayRobot(robot->getPosX(), robot->getPosY());
				Sleep(500);
			}
			isDone = false;
		}
	}

	//pathFinder->runOnePath(result);
	robot->calibrateAtStart();
	cout << "Wait for android run?";
	conn->waitForAndroidRun();
	result = pathFinder->findPathBetween(robot->getPosX(), robot->getPosY(), ARENA_END_X, ARENA_END_Y, true);
	pathFinder->runOnePath(result);

	io->printArena(arena);
	io->generateMapDescriptorLevel1a();
	io->generateMapDescriptorLevel2a();
	io->complementMap(arena);
	io->printArena(arena);
	io->generateMapDescriptorLevel1();
	io->generateMapDescriptorLevel2();
}

bool MainWindow::refreshRobot()
{
	
}

void MainWindow::hideRobot(int x, int y)
{
	Gdk::Color unoccupied("white");
	gridDisplay[x][y]->modify_bg(Gtk::StateType::STATE_NORMAL, unoccupied);
	gridDisplay[x+1][y]->modify_bg(Gtk::StateType::STATE_NORMAL, unoccupied);
	gridDisplay[x][y+1]->modify_bg(Gtk::StateType::STATE_NORMAL, unoccupied);
	gridDisplay[x+1][y+1]->modify_bg(Gtk::StateType::STATE_NORMAL, unoccupied);
}

void MainWindow::displayRobot(int x, int y)
{
	Gdk::Color robotColor("#263C8B");
	gridDisplay[x][y]->modify_bg(Gtk::StateType::STATE_NORMAL, robotColor);
	gridDisplay[x+1][y]->modify_bg(Gtk::StateType::STATE_NORMAL, robotColor);
	gridDisplay[x][y+1]->modify_bg(Gtk::StateType::STATE_NORMAL, robotColor);
	gridDisplay[x+1][y+1]->modify_bg(Gtk::StateType::STATE_NORMAL, robotColor);
}

void MainWindow::updateRobotDisplayLocation()
{

}

// Display the arena based on current information
void MainWindow::refreshDisplay()
{
	Gdk::Color unoccupied("white"), obstacle("#2E231F"), robotColor("#263C8B"), unexplored("#BDBF78"), startAndEnd("#BFA524");
	// change Arena
	
	for (std::map<int, int>::iterator i = arena->gridToRefresh->begin(); i != arena->gridToRefresh->end(); ++i)
	{
		int locateX = i->first, locateY = i->second;
		if (locateX < 0 || locateX >= ARENA_X_SIZE || locateY < 0 || locateY >= ARENA_Y_SIZE)
			continue;
		switch(arena->getGridType(locateX, locateY))
		{
		case UNOCCUPIED:
		case UNSAFE:
			gridDisplay[locateX][locateY]->modify_bg(Gtk::StateType::STATE_NORMAL, unoccupied);
			break;
		case OBSTACLE:
			gridDisplay[locateX][locateY]->modify_bg(Gtk::StateType::STATE_NORMAL, obstacle);
			break;
		case UNEXPLORED:
			gridDisplay[locateX][locateY]->modify_bg(Gtk::StateType::STATE_NORMAL, unexplored);
			break;
		case START:
		case GOAL:
			gridDisplay[locateX][locateY]->modify_bg(Gtk::StateType::STATE_NORMAL, startAndEnd);
			break;
		}
	}
	//change robot
	displayRobot(robot->getPosX(), robot->getPosY());
	arena->gridToRefresh->clear();
}

MainWindow::~MainWindow()
{  }