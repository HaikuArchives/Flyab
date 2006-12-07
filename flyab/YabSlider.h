#ifndef YABSLIDER_H
#define YABSLIDER_H

#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
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

		labelsize(B_FONT_SIZE);
		copy_label(label);
		step(1);
		color(fl_rgb_color(B_GREY));
	}

	void LabelTop()
	{
		copy_label(label);
		int lh = (int)labelsize();

		if (type() == FL_HORIZONTAL)
			resize(x, y+lh, w, h);
		else
			resize(x, y+lh, w, h-lh);
	}

	void setlabels(const char* label1, const char* label2)
	{
		copy_label(label1);
	}

private:
	int x, y, w, h;
	const char* label;
	Fl_Box *box;

};

#endif
