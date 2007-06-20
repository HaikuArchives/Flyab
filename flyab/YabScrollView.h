#ifndef YABSCROLLVIEW_H
#define YABSCROLLVIEW_H

#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
//#include <FL/Fl_Slider.H>
#include <FL/Fl_Scrollbar.H>
#include <string>
#include "YabView.h"
#include "YabWidget.h"
#include "global.h"

class YabScrollView: public Fl_Scrollbar, public YabWidget
{
public:
	YabScrollView(int x1,int y1,int width,int height, const char* id, int format_)
		: Fl_Scrollbar(width,y1,15,height), YabWidget(id)
	{
		x = x1; y = y1;
		w = width;
		h = height;
		format = format_;
		value(0, 70,0, h);

		/*
		YabView *view = new YabView(x, y, w, h, id);
		view->end();
		add(view);
		redraw();
		
		
		if (type() == FL_HORIZONTAL)
			resize(x, y, w, 15);
		*/
	}


private:
	int x, y, w, h, format;

};

#endif
