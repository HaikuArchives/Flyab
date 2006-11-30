#ifndef YABCOLORCONTROL_H
#define YABCOLORCONTROL_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Widget.H>
#include "YabWidget.h"

class YabColorControl: public Fl_Group, public YabWidget
{
public:
	YabColorControl(int x, int y, int w, int h, const char* id);
	~YabColorControl();
	void rgb(int r, int g, int b);
	void draw();
	double r();
	double g();
	double b();

private:
	Fl_Slider *slider[3];
	Fl_Input *txt[3];
	int bc[3], data[3];
	static void cb_common(Fl_Widget *widget, void *data);
	void cb_slider(Fl_Slider *slider, int n);
	void cb_input(Fl_Input *input, int n);
};
#endif
