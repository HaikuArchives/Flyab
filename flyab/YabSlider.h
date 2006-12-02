#ifndef YABSLIDER_H
#define YABSLIDER_H

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

		copy_label(label);
		step(1);
		color(fl_rgb_color(B_GREY));
		labelsize(B_FONT_SIZE);
	}

	void LabelTop()
	{
		copy_label(label);
		align(FL_ALIGN_TOP);
		int lh = fl_height()-2;
		resize(x, y+lh, w, h-lh);
	}

	void setlabels(const char* label1, const char* label2)
	{
		copy_label(label1);

/*		if (strcmp(label2, "") != 0)
		{
			int lw, lh;
			fl_measure(label2, lw, lh);
			if (!box)
			{
				box = new Fl_Box(w-lw, y, lw, lh);
				add(box);
			}
			else
				box->resize(w-lw, y, lw, lh);

			box->copy_label(label2);
		}
*/
	}

private:
	int x, y, w, h;
	const char* label;
	Fl_Box *box;

};

#endif
