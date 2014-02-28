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
	ofstream mapFile;
	mapFile.open (filename);


	mapFile.close();
}


void MapIO::generateMapDescriptor()
{

}
