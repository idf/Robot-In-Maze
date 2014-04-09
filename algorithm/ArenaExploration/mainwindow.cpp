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
	percentageEntry.set_text("100");
	timeLimitEntry.set_text("400");
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

MainWindow::~MainWindow()
{  }

void MainWindow::startExplorationButtonClicked()
{
	robot->senseEnvironment(arena, fullArena);
	io->printArena(arena);
#ifdef ANDROID
	conn->waitForAndroidExplore();
#endif
#ifdef GUI
	this->refreshAllDisplay();
#endif
	pathFinder->start = time(0);
	Glib::signal_idle().connect( sigc::mem_fun(*this, &MainWindow::exploreProcessHandler) );
}

bool MainWindow::exploreProcessHandler()
{
	//robot->calibrateAtStart();
	std::vector<std::pair<std::string, int>*>* movementList = new std::vector<std::pair<std::string, int>*>();
	//movementList->push_back(new pair<string, int>("rotateClockwise", 90));
	//movementList->push_back(new pair<string, int>("moveForward", 30));
	//movementList->push_back(new pair<string, int>("rotateClockwise", 90));
	//movementList->push_back(new pair<string, int>("moveForward", 30));
	//movementList->push_back(new pair<string, int>("rotateCounterClockwise", 90));
	//movementList->push_back(new pair<string, int>("moveForward", 80));
	//movementList->push_back(new pair<string, int>("rotateClockwise", 90));
	//movementList->push_back(new pair<string, int>("moveForward", 60));
	//movementList->push_back(new pair<string, int>("rotateCounterClockwise", 90));
	//movementList->push_back(new pair<string, int>("moveForward", 60));
	//movementList->push_back(new pair<string, int>("rotateClockwise", 90));
	//movementList->push_back(new pair<string, int>("moveForward", 30));
	//pathFinder->runOnePath(movementList, true);
	//return false;
	std::vector<Grid*> result;
	bool continueTimer = pathFinder->explore(atoi(percentageEntry.get_text().c_str()), atoi(timeLimitEntry.get_text().c_str()));
#ifdef GUI
	this->refreshAllDisplay();
#endif
	io->printArena(arena);
	if (continueTimer)
	{
		while(!robot->sendItselfAndArena(arena))
			;
	}
	else  // exploration is done, go back to start point
	{
#ifdef GUI
		robot->calibrateAtGoal();
		result = pathFinder->findPathBetween(robot->getPosX(), robot->getPosY(), ARENA_START_X, ARENA_START_Y, true);
		hideRobot(robot->getPosX(), robot->getPosY());
		pathFinder->runOnePath(result);
		displayRobot(robot->getPosX(), robot->getPosY());
		robot->calibrateAtStart();
		getchar();

		
		result = pathFinder->findPathBetween(robot->getPosX(), robot->getPosY(), ARENA_END_X, ARENA_END_Y, true);
		movementList = pathFinder->getMovementList(result);
		for (vector<Grid*>::reverse_iterator i = result.rbegin(); i != result.rend(); i++)
			std::cout << (*i)->x << "," <<(*i)->y << endl;
		for (vector<pair<std::string, int>*>::iterator i = movementList->begin(); i != movementList->end(); i++)
			std::cout << (*i)->first << "," <<(*i)->second << endl;
		this->i = movementList->begin(); 
		waitDone = true;
		Glib::signal_timeout().connect( sigc::mem_fun(*this, &MainWindow::shortestPathHandler), 250 );
		return false;
#else
		if (robot->getPosX() != ARENA_START_X || robot->getPosY() != ARENA_START_Y)
		{
			if (robot->getPosX() == ARENA_END_X && robot->getPosY() == ARENA_END_Y)
				robot->calibrateAtGoal();
			result = pathFinder->findPathBetween(robot->getPosX(), robot->getPosY(), ARENA_START_X, ARENA_START_Y, true);
			movementList = pathFinder->getMovementList(result);
			pathFinder->runOnePath(movementList, false);
		}
		robot->calibrateAtStart();
		result = pathFinder->findPathBetween(robot->getPosX(), robot->getPosY(), ARENA_END_X, ARENA_END_Y, true);
		movementList = pathFinder->getMovementList(result);
		pathFinder->runOnePath(movementList, true);
#endif
		io->printArena(arena);
		io->generateMapDescriptorLevel1a();
		io->generateMapDescriptorLevel2a();
		io->complementMap(arena);
		io->printArena(arena);
		io->generateMapDescriptorLevel1();
		io->generateMapDescriptorLevel2(); 
	}
	return continueTimer;
}

// GUI Methods
#ifdef GUI
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
#endif