#include "mainwindow.h"
#include "MapIO.h"
#include "connector.h"
#include <iostream>
#include <windows.h>


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
#ifdef ANDROID
	conn->waitForAndroidExplore();
#endif
	robot->senseEnvironment(arena, fullArena);
	this->refreshDisplay();
	pathFinder->start = time(0);
	Glib::signal_idle().connect( sigc::mem_fun(*this, &MainWindow::exploreProcessHandler) );
}

bool MainWindow::exploreProcessHandler()
{
	bool continueTimer = false;
	continueTimer = pathFinder->explore(atoi(percentageEntry.get_text().c_str()), atoi(timeLimitEntry.get_text().c_str()));
	this->refreshAllDisplay();
	io->printArena(arena);
	while(!robot->sendItselfAndArena(arena))
		;
	
	if (!continueTimer)
	{
		robot->calibrateAtGoal();
		result = pathFinder->findPathBetween(robot->getPosX(), robot->getPosY(), ARENA_START_X, ARENA_START_Y, true);
		hideRobot(robot->getPosX(), robot->getPosY());
		pathFinder->runOnePath(result);
		displayRobot(robot->getPosX(), robot->getPosY());
		getchar();

		robot->calibrateAtStart();
		result = pathFinder->findPathBetween(robot->getPosX(), robot->getPosY(), ARENA_END_X, ARENA_END_Y, false);
		movementList = pathFinder->getMovementList(result);
		for (vector<Grid*>::reverse_iterator i = result.rbegin(); i != result.rend(); i++)
			std::cout << (*i)->x << "," <<(*i)->y << endl;
		for (vector<pair<std::string, int>*>::iterator i = movementList->begin(); i != movementList->end(); i++)
			std::cout << (*i)->first << "," <<(*i)->second << endl;
		this->i = movementList->begin(); 
		waitDone = true;
		Glib::signal_timeout().connect( sigc::mem_fun(*this, &MainWindow::shortestPathHandler), 250 );
		return false;

		//robot->calibrateAtStart();
#ifdef ANDROID
		//cout << "Wait for android run?";
		//conn->waitForAndroidRun();
#endif
		//result = pathFinder->findPathBetween(robot->getPosX(), robot->getPosY(), ARENA_END_X, ARENA_END_Y, true);
		//pathFinder->runOnePath(result);
		//
		//io->printArena(arena);
		//io->generateMapDescriptorLevel1a();
		//io->generateMapDescriptorLevel2a();
		//io->complementMap(arena);
		//io->printArena(arena);
		//io->generateMapDescriptorLevel1();
		//io->generateMapDescriptorLevel2(); 
	}
	return continueTimer;
}
bool MainWindow::shortestPathHandler()
{
	if (i <= movementList->end())
	{
		if (!waitDone)
		{
			hideRobot(robot->getPosX(), robot->getPosY());
			waitDone = robot->moveForwardWithDisplay(range);
			displayRobot(robot->getPosX(), robot->getPosY());
			return true;
		}
		if (i == movementList->end())
			return false;
		if((*i)->first == "rotateClockwise")
		{
			robot->rotateClockwise((*i)->second);
			Sleep(2000);
		}
		else if ((*i)->first == "rotateCounterClockwise")
		{
			robot->rotateCounterClockwise((*i)->second);
			Sleep(2000);
		}
		else
		{
			hideRobot(robot->getPosX(), robot->getPosY());
			range = (*i)->second;
			waitDone = robot->moveForwardWithDisplay(range);
			displayRobot(robot->getPosX(), robot->getPosY());
			Sleep(2000);
		}
		++i;
		return true;
	}
	else
		return false;
	
}

void MainWindow::hideRobot(int x, int y)
{
	Gdk::Color unoccupied("white");
	cout <<"hide: " << x << ", " << y << endl;
	gridDisplay[x][y]->modify_bg(Gtk::StateType::STATE_NORMAL, unoccupied);
	gridDisplay[x+1][y]->modify_bg(Gtk::StateType::STATE_NORMAL, unoccupied);
	gridDisplay[x][y+1]->modify_bg(Gtk::StateType::STATE_NORMAL, unoccupied);
	gridDisplay[x+1][y+1]->modify_bg(Gtk::StateType::STATE_NORMAL, unoccupied);
	this->show_all();
}

void MainWindow::displayRobot(int x, int y)
{
	Gdk::Color robotColor("#263C8B");
	cout <<"show: " << x << ", " << y << endl;
	gridDisplay[x][y]->modify_bg(Gtk::StateType::STATE_NORMAL, robotColor);
	gridDisplay[x+1][y]->modify_bg(Gtk::StateType::STATE_NORMAL, robotColor);
	gridDisplay[x][y+1]->modify_bg(Gtk::StateType::STATE_NORMAL, robotColor);
	gridDisplay[x+1][y+1]->modify_bg(Gtk::StateType::STATE_NORMAL, robotColor);
	this->show_all();
}

// Display the arena based on current information
void MainWindow::refreshDisplay()
{
	Gdk::Color unoccupied("white"), obstacle("#2E231F"), robotColor("#263C8B"), unexplored("#BDBF78"), startAndEnd("#BFA524");
	// change Arena
	
	for (std::map<int, int>::iterator i = arena->gridToRefresh->begin(); i != arena->gridToRefresh->end(); ++i)
	{
		int locateX = i->first, locateY = i->second;
		cout << "refresh: " << locateX << ", " << locateY <<endl;
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


void MainWindow::refreshAllDisplay()
{
	Gdk::Color unoccupied("white"), obstacle("#2E231F"), robotColor("#263C8B"), unexplored("#BDBF78"), startAndEnd("#BFA524");
	// change Arena
	
	for (int i = 0; i < ARENA_X_SIZE; ++i)
	{
		for (int j = 0; j < ARENA_Y_SIZE; ++j)
		{
			int locateX = i, locateY = j;
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
	}
	//change robot
	displayRobot(robot->getPosX(), robot->getPosY());
}
MainWindow::~MainWindow()
{  }