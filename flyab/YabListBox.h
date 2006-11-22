#ifndef YABLISTBOX_H
#define YABLISTBOX_H

#include <FL/Fl_Hold_Browser.H>
#include "YabWidget.h"

class YabListBox: public Fl_Hold_Browser, public YabWidget
{
public:
	YabListBox(int x1, int y1, int width, int height, const char* id)
		: Fl_Hold_Browser(x1,y1,width,height), YabWidget(id)
	{
	}

	int handle(int event)
	{
		if(Fl::event_key(FL_Enter))
			do_callback();
		else
			return Fl_Hold_Browser::handle(event);
	}
};

#endif
