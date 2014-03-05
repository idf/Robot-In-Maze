#include "displayItem.h"

DisplayItem::DisplayItem(int x, int y)
	:x(x), y(y)
{ 
	this->set_size_request(ITEM_PIXEL_SIZE, ITEM_PIXEL_SIZE);
}

DisplayItem::~DisplayItem()
{
}