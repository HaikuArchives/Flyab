#ifndef YABCOLORCONTROL_H
#define YABCOLORCONTROL_H

//#include <FL/fl_draw.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Slider.H>
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
	double c[3];
	int bc[3];
};
#endif
