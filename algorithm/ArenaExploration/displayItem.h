#ifndef DISPLAYITEM_H
#define DISPLAYITEM_H

#include <gtkmm.h>

class DisplayItem : public Gtk::EventBox
{
public:
	DisplayItem(int x, int y);
	virtual ~DisplayItem();
	int x, y;
};

#endif