#ifndef DISPLAYITEM_H
#define DISPLAYITEM_H

#define ITEM_PIXEL_SIZE 30

#include <gtkmm.h>

class DisplayItem : public Gtk::EventBox
{
public:
	DisplayItem(int x, int y);
	virtual ~DisplayItem();
	int x, y;
};

#endif