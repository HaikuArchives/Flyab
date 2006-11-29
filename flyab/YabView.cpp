#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <iostream>
#include "YabView.h"
#include "global.h"
	
YabView::YabView(int x, int y, int width, int height, const char* id)
	: Fl_Group(x,y,width,height), YabWidget(id)
{
	hasmenu = false;
}

YabView::~YabView()
{
	for(int i=0; i<drawList.size(); i++)
	{
		YabDrawing *last = drawList[i];
		drawList[i] = NULL;
		if(last->command == 0) delete [] last->chardata;
		delete last;
	}
	drawList.clear();
}

void YabView::AddDrawing(YabDrawing* drawing)
{
	drawList.push_back(drawing);
}

void YabView::FlushDrawings()
{
	for(int i=0; i<drawList.size(); i++)
	{
		YabDrawing *last = drawList[i];
		drawList[i] = NULL;
		if(last->command == 0) delete [] last->chardata;
		delete last;
	}
	drawList.clear();
	// this might be a workaround as well, but with less lines ;P
	hide();
	show();
}

void YabView::draw()
{
	fl_color(0,0,0);
	for(int i=0; i<drawList.size(); i++)
	{
		YabDrawing *e = drawList[i];
		switch(e->command)
		{
			case 0: // draw text
				fl_draw(e->chardata, e->x1, e->y1);
				break;
			case 1: // draw line
				fl_line(e->x1, e->y1, e->x2, e->y2); 
				break;
			default:
				break;
		}
	}
	Fl_Group::draw();
}

bool YabView::HasMenu()
{
	return hasmenu;
}

void YabView::HasMenu(bool m)
{
	hasmenu = m;
}

