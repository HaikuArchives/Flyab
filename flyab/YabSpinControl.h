#ifndef YABSPINCONTROL_H
#define YABSPINCONTROL_H

#include <FL/Fl_Spinner.H>
#include <string>
#include "YabWidget.h"

class YabSpinControl: public Fl_Spinner, public YabWidget
{
public:
	YabSpinControl(int x, int y, const char* id, const char* label)
		: Fl_Spinner(x+13,y,40,20), YabWidget(id)
	{
		copy_label(label);
	}

};

#endif
