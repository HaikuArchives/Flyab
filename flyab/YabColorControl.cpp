#include <FL/Fl.H>
#include "YabColorControl.h"

YabColorControl::YabColorControl(int x, int y, int w, int h, const char* id)
	:Fl_Group(x, y, w, h), YabWidget(id)
{
	bc[0] = fl_rgb_color(254, 100, 100);
	bc[1] = fl_rgb_color(100, 254, 100);
	bc[2] = fl_rgb_color(100, 100, 254);
	int sh=h/3, min=0, max=255;

	for (int i=0; i<3; i++)
	{
		slider[i] = new Fl_Slider(x, y+(i*sh), w, sh);
		slider[i]->type(FL_HORIZONTAL);
		slider[i]->slider_size(0.05);
		slider[i]->minimum(min);
		slider[i]->maximum(max);
		slider[i]->step(1);
		slider[i]->value(0);
		slider[i]->color(bc[i]);
		slider[i]->redraw();
		add(slider[i]);
	}
	resizable(NULL);
	end();
	redraw();
}
YabColorControl::~YabColorControl()
{
	for (int i=0; i<3; i++)
	{
		delete slider[i];
	}
}

void YabColorControl::rgb(int r, int g, int b)
{
	c[0] = r;
	c[1] = g;
	c[2] = b;
	for (int i=0; i<3; i++)
	{
		slider[i]->value(c[i]);
		slider[i]->redraw();
	}
}

double YabColorControl::r()
{
	return slider[0]->value();
}

double YabColorControl::g()
{
	return slider[1]->value();
}

double YabColorControl::b()
{
	return slider[2]->value();
}

void YabColorControl::draw()
{
	Fl_Group::draw();
}
