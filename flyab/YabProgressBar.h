#ifndef YABPROGRESSBAR_H
#define YABPROGRESSBAR_H

#include <FL/Fl_Progress.H>
#include "YabWidget.h"

class YabProgressBar: public Fl_Progress, public YabWidget
{
public:
	YabProgressBar(const char* id, int x, int y, int w, int h, const char* label)
		:Fl_Progress(x, y, w, h, label), YabWidget(id)
	{
		color(fl_rgb_color(B_GREY));
		selection_color(fl_rgb_color(66, 170, 75));
	}
	~YabProgressBar()
	{
	}
};

#endif
