#ifndef YABVIEW_H
#define YABVIEW_H

#include <FL/Fl_Group.H>
#include <string>
#include "YabWidget.h"

class YabView: public Fl_Group, public YabWidget
{
public:
	YabView(int x1, int y1, int width, int height, const char* id);
	void draw();

};

#endif
