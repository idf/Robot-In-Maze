#ifndef MAPIO_H
#define MAPIO_H

#include <iostream>
#include <string>
#include <vector>
#include "Arena.h"

class MapIO
{
public:
	MapIO(Arena* arena);
	MapIO::MapIO(Arena* arena, Arena* fullArena);
	MapIO::~MapIO();
	void MapIO::printArena(Arena* arena);
	void MapIO::printPath(std::vector<Grid*> path);
	void readMapFromFile(std::string filename);
	void generateMapDescriptorLevel1a();
	void generateMapDescriptorLevel2a();
	void generateMapDescriptorLevel1();
	void generateMapDescriptorLevel2();
	void complementMap(Arena* arena);
private:
	Arena* _arena;
	Arena* _fullArena;
	std::string convertToHex(std::string binaryStr);
	char MapIO::toHexDigit(int value);
};

#endif