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
	conn->waitForAndroidExplore();
	robot->senseEnvironment(arena, fullArena);
#ifdef HARDWARE
	Glib::signal_idle().connect( sigc::mem_fun(*this, &MainWindow::exploreProcessHandler));
#else
	Glib::signal_timeout().connect( sigc::mem_fun(*this, &MainWindow::exploreProcessHandler), 500);
#endif
	pathFinder->start = time(0);
}

bool MainWindow::startGoToDestination()
{
	cout << "running";
	return true;
}

bool MainWindow::exploreProcessHandler()
{
	//pathFinder->runOnePath(pathFinder->findPathBetween(ARENA_START_X, ARENA_START_Y, ARENA_END_X, ARENA_END_Y));
	//robot->calibrateAtGoal();
	//pathFinder->runOnePath(pathFinder->findPathBetween(ARENA_END_X, ARENA_END_Y, ARENA_START_X, ARENA_START_Y));
	//return false;

	bool continueTimer = pathFinder->explore(atoi(percentageEntry.get_text().c_str()), atoi(timeLimitEntry.get_text().c_str()));
	this->refreshDisplay();
	io->printArena(arena);
	while(!robot->sendItselfAndArena(arena))
		;
	if (!continueTimer)
	{
		// trick
		for (int i = 0; i < ARENA_X_SIZE; ++i)
		{
			for (int j = 0; j < ARENA_Y_SIZE; ++j)
			{
				if (arena->getGridType(i, j) == UNEXPLORED)
					arena->setGridType(i, j, UNOCCUPIED);
			}
		}
		io->generateMapDescriptorLevel1();
		io->generateMapDescriptorLevel2();
	}
	return continueTimer;
}

// to be done
// in hardware level
bool MainWindow::shortestPathHandler()
{
	return true;
}

// Display the arena based on current information
void MainWindow::refreshDisplay()
{
	Gdk::Color unoccupied("white"), obstacle("#2E231F"), robotColor("#263C8B"), pathColor("#4E74A6"), startAndEnd("#BFA524");
	// change Arena
	
	for (std::map<int, int>::iterator i = arena->gridToRefresh->begin(); i != arena->gridToRefresh->end(); ++i)
	{
		int locateX = (*i).first, locateY = (*i).second;
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
			//gridDisplay[locateX][locateY]->modify_bg(Gtk::StateType::STATE_NORMAL, unexplored);
			break;
		case START:
		case GOAL:
			gridDisplay[locateX][locateY]->modify_bg(Gtk::StateType::STATE_NORMAL, startAndEnd);
			break;
		}
	}
	//for (int i = -1; i < 3; ++i)
	//{
	//	for (int j = -1; j < 3; ++j)
	//	{
	//		int locateX = robot->getPosX()+i, locateY = robot->getPosY()+j;
	//		if (locateX < 0 || locateX >= ARENA_X_SIZE || locateY < 0 || locateY >= ARENA_Y_SIZE)
	//			continue;
	//		switch(arena->getGridType(locateX, locateY))
	//		{
	//		case UNOCCUPIED:
	//		case UNSAFE:
	//			gridDisplay[locateX][locateY]->modify_bg(Gtk::StateType::STATE_NORMAL, unoccupied);
	//			break;
	//		case OBSTACLE:
	//			gridDisplay[locateX][locateY]->modify_bg(Gtk::StateType::STATE_NORMAL, obstacle);
	//			break;
	//		case UNEXPLORED:
	//			//gridDisplay[locateX][locateY]->modify_bg(Gtk::StateType::STATE_NORMAL, unexplored);
	//			break;
	//		case START:
	//		case GOAL:
	//			gridDisplay[locateX][locateY]->modify_bg(Gtk::StateType::STATE_NORMAL, startAndEnd);
	//			break;
	//		}
	//	}
	//}


	//change robot
	gridDisplay[robot->getPosX()][robot->getPosY()]->modify_bg(Gtk::StateType::STATE_NORMAL, robotColor);
	gridDisplay[robot->getPosX()+1][robot->getPosY()]->modify_bg(Gtk::StateType::STATE_NORMAL, robotColor);
	gridDisplay[robot->getPosX()][robot->getPosY()+1]->modify_bg(Gtk::StateType::STATE_NORMAL, robotColor);
	gridDisplay[robot->getPosX()+1][robot->getPosY()+1]->modify_bg(Gtk::StateType::STATE_NORMAL, robotColor);
	arena->gridToRefresh->clear();
}

MainWindow::~MainWindow()
{  }