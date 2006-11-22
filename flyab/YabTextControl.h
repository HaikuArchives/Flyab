#ifndef YABTEXTCONTROL_H
#define YABTEXTCONTROL_H

#include <FL/Fl_Input.H>
#include "YabWidget.h"

class YabTextControl: public Fl_Input, public YabWidget
{
public:
	YabTextControl(int x, int y, int w, int h, const char* id, const char* label)
		: Fl_Input(x,y,w,h), YabWidget(id)
	{
		copy_label(label);
	}

};

#endif
