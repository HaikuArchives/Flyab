#ifndef YABRADIOBUTTON_H
#define YABRADIOBUTTON_H

#include <FL/Fl_Round_Button.H>
#include <string>
#include "YabWidget.h"

class YabRadioButton: public Fl_Round_Button, public YabWidget
{
public:
	YabRadioButton(int x, int y, const char* id, const char* label)
		: Fl_Round_Button(x,y,17,14), YabWidget(id)
	{
		copy_label(label);
	}

};

#endif
