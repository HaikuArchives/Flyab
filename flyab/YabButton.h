#ifndef YABBUTTON_H
#define YABBUTTON_H

#include <FL/Fl_Button.H>
#include <string>
#include "YabWidget.h"

class YabButton: public Fl_Button, public YabWidget
{
public:
	YabButton(int x1, int y1, int width, int height, const char* id, const char* label)
		: Fl_Button(x1,y1,width,height), YabWidget(id)
	{
		copy_label(label);
	}

};

#endif
