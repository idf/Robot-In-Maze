#include "MapIO.h"
#include <fstream>

using namespace std;

MapIO::MapIO(Arena* arena)
{
	_arena = arena;
}

MapIO::~MapIO()
{

}

void MapIO::readMapFromFile(std::string filename)
{
	ifstream mapFile;
	string data;
	mapFile.open (filename);
	while(!mapFile.eof)
	{
		int count = 0;
		getline(mapFile, data);
		for (int i = 0; i < data.length; ++i)
		{
			switch (*i)
			{
			case '0':
				_arena->setGridType(i, count, UNOCCUPIED);
				break;
			case '1':
				_arena->setGridType(i, count, OBSTACLE);
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
