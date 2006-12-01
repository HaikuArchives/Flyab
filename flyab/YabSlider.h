#ifndef YABSLIDER_H
#define YABSLIDER_H

#include <FL/Fl_Slider.H>
#include <string>
#include "YabWidget.h"
#include "global.h"

class YabSlider: public Fl_Slider, public YabWidget
{
public:
	YabSlider(int x1, int y1, int width, int height, const char* id, const char* label_)
		: Fl_Slider(x1,y1,width,height), YabWidget(id)
	{
		x = x1; y = y1;
		w = width;
		h = height;
		label = label_;
		copy_label(label);
		step(1);
		color(fl_rgb_color(B_GREY));
		labelsize(B_FONT_SIZE);
	}

	void LabelLeft()
	{
		copy_label(label);
		align(FL_ALIGN_LEFT);
		int lw = 5+(int)fl_width(label);
		resize(x+lw, y, w-lw, h);
	}

	void LabelRight()
	{
		copy_label(label);
		align(FL_ALIGN_RIGHT);
		int lw = 5+(int)fl_width(label);
		resize(x, y, w-lw, h);
	}

	void LabelTop()
	{
		copy_label(label);
		align(FL_ALIGN_TOP);
		int lh = fl_height()-2;
		resize(x, y+lh, w, h-lh);
	}

private:
	int x, y, w, h;
	const char* label;

};

#endif
