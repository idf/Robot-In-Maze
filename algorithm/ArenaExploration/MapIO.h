#ifndef MAPIO_H
#define MAPIO_H

#include <iostream>
#include <string>
#include "Arena.h"

class MapIO
{
public:
	MapIO(Arena* arena);
	MapIO::~MapIO();
	void readMapFromFile(std::string filename);
	void generateMapDescriptor();

private:
	Arena* _arena;
};

#endif