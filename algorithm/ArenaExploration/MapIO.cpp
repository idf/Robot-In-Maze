#include "MapIO.h"
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <string>

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

// generate level 1 map descriptor
// covered and uncovered
void MapIO::generateMapDescriptorLevel1()
{
	ofstream mapDescriptor1("descriptor1.txt");
	string data("");
	data.append("11");

	for (int i = ARENA_X_SIZE - 1; i >= 0; --i)
	{
		for (int j = 0; j < ARENA_Y_SIZE; ++j)
		{
			switch(_arena->getGridType(i, j))
			{
			case UNEXPLORED:
				data.append("0");
				break;
			default:
				data.append("1");
				break;
			}
		}
	}
	data.append("11");

	string hexData = convertToHex(data);
	mapDescriptor1 << hexData;
	mapDescriptor1.close();
}



void MapIO::generateMapDescriptorLevel2()
{
	ofstream mapDescriptor2("descriptor2.txt");
	string data("");
	data.append("11");

	for (int i = ARENA_X_SIZE - 1; i >= 0; --i)
	{
		for (int j = 0; j < ARENA_Y_SIZE; ++j)
		{
			switch(_arena->getGridType(i, j))
			{
			case UNOCCUPIED:
			case START:
			case GOAL:
				data.append("0");
				break;
			case OBSTACLE:
				data.append("1");
				break;
			default:
				break;
			}
		}
	}
	data.append("11");

	string hexData = convertToHex(data);
	mapDescriptor2 << hexData;
	mapDescriptor2.close();
}

std::string MapIO::convertToHex(std::string binaryStr)
{
	string temp(""), hexStr("");
	string::iterator i = binaryStr.begin();
	int count = 0;
	while (i < binaryStr.end())
	{
		if (count == 4)
		{
			hexStr += toHexDigit(atoi(temp.c_str()));
			temp.clear();
			count = 0;
		}
		else
		{
			temp += *i;
			count++;
			i++;
		}
	}

	while (temp.length() < 4)
		temp += '0';
	hexStr += toHexDigit(atoi(temp.c_str()));
	return hexStr;
}

char MapIO::toHexDigit(int value)
{
	switch(value)
	{
	case 0:
		return '0'; break;
	case 1:
		return '1'; break;	
	case 10:
		return '2'; break;	
	case 11:
		return '3'; break;	
	case 100:
		return '4'; break;	
	case 101:
		return '5'; break;	
	case 110:
		return '6'; break;	
	case 111:
		return '7'; break;	
	case 1000:
		return '8'; break;	
	case 1001:
		return '9'; break;	
	case 1010:
		return 'A'; break;	
	case 1011:
		return 'B'; break;	
	case 1100:
		return 'C'; break;	
	case 1101:
		return 'D'; break;	
	case 1110:
		return 'E'; break;	
	case 1111:
		return 'F'; break;	
	}
}