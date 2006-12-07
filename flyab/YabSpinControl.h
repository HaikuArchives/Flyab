#ifndef YABSPINCONTROL_H
#define YABSPINCONTROL_H

#include <FL/Fl_Spinner.H>
#include <string>
#include "YabWidget.h"

class YabSpinControl: public Fl_Spinner, public YabWidget
{
public:
	YabSpinControl(int x, int y, int max, const char* id, const char* label)
		: Fl_Spinner(x,y,40,20), YabWidget(id)
	{
		char text[10];
		sprintf(text, "%d", max);

		color(fl_rgb_color(B_GREY));
		labelsize(B_FONT_SIZE);
		textsize(B_FONT_SIZE);
		copy_label(label);
		int lw = (int)fl_width(label);
		int tw = 18+(int)fl_width(text);
		resize(x+lw, y, tw, 20);
	}

};

#endif
