#include <FL/fl_draw.H>
#include "YabView.h"
#include <iostream>
	
YabView::YabView(int x, int y, int width, int height, const char* id)
	: Fl_Group(x,y,width,height), YabWidget(id)
{
}

void YabView::draw()
{
	Fl_Group::draw();
}

