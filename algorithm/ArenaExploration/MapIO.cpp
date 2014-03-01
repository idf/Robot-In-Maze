#include "MapIO.h"
#include <fstream>

using namespace std;

MapIO::MapIO(Arena* arena)
{
	_arena = arena;
	_fullArena = NULL;
}

MapIO::MapIO(Arena* arena, Arena* fullArena)
{
	_arena = arena;
	_fullArena = fullArena;
}

MapIO::~MapIO()
{
}

void MapIO::printArena(Arena* arena)
{
	for (int i = 0; i < ARENA_Y_SIZE; ++i)
	{
		for (int j = 0; j < ARENA_X_SIZE; ++j)
			cout << arena->getGridType(j, i);
		cout << endl;
	}
}

void MapIO::printPath(std::vector<Grid*> path)
{
	for (std::vector<Grid*>::reverse_iterator i = path.rbegin(); i != path.rend(); ++i)
	{
		cout << (*i)->x << "," << (*i)->y << endl;
	}
}

void MapIO::readMapFromFile(std::string filename)
{
	ifstream mapFile;
	string data;
	mapFile.open (filename);
	int count = 0;
	while(!mapFile.eof())
	{
		getline(mapFile, data);
		for (int i = 0; i < data.length(); ++i)
		{
			switch (data[i])
			{
			case '0':
				if (_fullArena->getGridType(i, count) != START && _fullArena->getGridType(i, count) != GOAL)
					_fullArena->setGridType(i, count, UNOCCUPIED);
				break;
			case '1':
				_fullArena->setGridType(i, count, OBSTACLE);
				break;
			}
		}
		++count;
	}
	mapFile.close();
}


void MapIO::generateMapDescriptorLevel1()
{

}

void MapIO::generateMapDescriptorLevel2()
{

}
