#include <FL/Fl.H>
#include "YabColorControl.h"
#include "global.h"

YabColorControl::YabColorControl(int x, int y, int w, int h, const char* id)
	:Fl_Group(x, y, w, h), YabWidget(id)
{
	min=0; max=255;
	bc[0] = fl_rgb_color(254, 100, 100);
	bc[1] = fl_rgb_color(100, 254, 100);
	bc[2] = fl_rgb_color(100, 100, 254);

	int tw = 30;
	int sh=h/3;

	for (int i=0; i<3; i++)
	{
		data[i] = i;
		slider[i] = new Fl_Slider(x, y+(i*sh), w-tw, sh);
		slider[i]->type(FL_HORIZONTAL);
		slider[i]->slider_size(0.05);
		slider[i]->minimum(min);
		slider[i]->maximum(max);
		slider[i]->step(1);
		slider[i]->value(0);
		slider[i]->color(bc[i]);
//		slider[i]->redraw();
		slider[i]->callback(cb_common, (void *)data[i]);
//		add(slider[i]);

		txt[i] = new Fl_Input(x+w-tw, y+(i*sh), tw, sh);
		txt[i]->type(FL_INT_INPUT);
		txt[i]->value("0");
		txt[i]->textsize(12);
		txt[i]->when(FL_WHEN_ENTER_KEY);
		txt[i]->callback(cb_common, (void *)data[i]);
//		add(txt[i]);
	}
	init_sizes();
	end();
	resizable(NULL);
	redraw();
}

YabColorControl::~YabColorControl()
{
	for (int i=0; i<3; i++)
	{
		delete slider[i];
		delete txt[i];
	}
}

void YabColorControl::rgb(int r, int g, int b)
{
	char val_txt[4];
	int c[3]={r, g, b};
	for (int i=0; i<3; i++)
	{
		slider[i]->value(c[i]);
		slider[i]->redraw();
		sprintf(val_txt, "%d", c[i]);
		txt[i]->value(val_txt);
		txt[i]->redraw();
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

void YabColorControl::cb_slider(Fl_Slider *slider, int n)
{
	char val_txt[4];
	int val_num = (int)slider->value();
	sprintf(val_txt, "%d", val_num);
	txt[n]->value(val_txt);
	txt[n]->redraw();
	return;
}

void YabColorControl::cb_input(Fl_Input *input, int n)
{
	bool is_wrong=false;
	int v = atoi(input->value());

	if (v > max || v < min) is_wrong = true;
	if (v > max) v = max;
	if (v < min) v = min;

	if (is_wrong)
	{
		char val_txt[4];
		sprintf(val_txt, "%d", v);
		input->value(val_txt);
		input->redraw();
	}
	slider[n]->value(v);
	slider[n]->redraw();
}

void YabColorControl::cb_common(Fl_Widget *widget, void *data)
{
	if (Fl_Slider *slider = dynamic_cast<Fl_Slider*>(widget))
		((YabColorControl*)widget->parent())->cb_slider(slider, (int)data);
	if (Fl_Input *input = dynamic_cast<Fl_Input*>(widget))
		((YabColorControl*)widget->parent())->cb_input(input, (int)data);
}

