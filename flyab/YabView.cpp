#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <iostream>
#include "YabView.h"
#include "global.h"
	
YabView::YabView(int x, int y, int width, int height, const char* id)
	: Fl_Group(x,y,width,height), YabWidget(id)
{
	flush = false;
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
	flush = true;
}

void YabView::draw()
{
	fl_color(0,0,0);
	if(flush)
	{ 
		// uebler Workaround, aber funktioniert erstmal
		fl_draw_box(FL_FLAT_BOX, 0,0, (window())->w(),(window())->h(), fl_rgb_color(B_GREY));
		flush = false;
		draw();
	}
	else
	{
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
	}
	Fl_Group::draw();
}

