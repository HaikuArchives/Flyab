#include <FL/Fl_Menu_Button.H>
#include "YabWidget.h"

class YabDropBox: public Fl_Menu_Button, public YabWidget
{
public:
	YabDropBox(int x1, int y1, int width, int height, const char* id)
		: Fl_Menu_Button(x1,y1,width,height), YabWidget(id)
	{
	}

	int handle(int event)
	{
		if(Fl::event_key(FL_Enter))
			do_callback();
		else
			return Fl_Menu_Button::handle(event);
	}
};
