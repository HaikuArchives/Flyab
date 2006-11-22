#ifndef YABCHECKBUTTON_H
#define YABCHECKBUTTON_H

#include <FL/Fl_Check_Button.H>
#include <string>
#include "YabWidget.h"

class YabCheckButton: public Fl_Check_Button, public YabWidget
{
public:
	YabCheckButton(int x, int y, const char* id, const char* label)
		: Fl_Check_Button(x,y,17,14), YabWidget(id)
	{
		copy_label(label);
	}

};

#endif
