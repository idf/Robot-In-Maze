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
	void generateMapDescriptorLevel1();
	void generateMapDescriptorLevel2();
private:
	Arena* _arena;
};

#endif