#ifndef YABSLIDER_H
#define YABSLIDER_H

#include <FL/Fl_Slider.H>


#include <string>
#include "YabWidget.h"

class YabSlider: public Fl_Slider, public YabWidget
{
public:
	YabSlider(int x1, int y1, int width, int height, const char* id, const char* label)
		: Fl_Slider(x1,y1,width,height), YabWidget(id)
	{
		copy_label(label);
	}

};

#endif
