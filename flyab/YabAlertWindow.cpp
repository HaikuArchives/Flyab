#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include "global.h"
#include "YabAlertWindow.h"

YabAlertWindow::YabAlertWindow(const char* text, const char* button1, const char* button2, const char* button3, const char* type)
{
	using namespace std;
	int x,y,w,h;
	Fl::screen_xywh(x,y,w,h);
	x = w/2 - 205;
	y = h/2 - 50;
	win = new Fl_Window(x,y,310,100, "Alert");

	alertText = new YabAlertText(50,5,245, text);
	alertText->redraw();

	int atxth = alertText->GetHeight();
	int fontw=0,fonth=0, bx=310, bw=0, num=-1;
	const char* label[3];
	label[0] = button3;
	label[1] = button2;
	label[2] = button1;
	for (int i=0; i<3; i++)
	{
		button[i] = NULL;
		if (label[i] == NULL) continue;

		fl_measure(label[i], fontw, fonth);
		bx = bx-fontw-40;
		bw = fontw+30;
		num++;

		button[num] = new Fl_Button(bx, atxth+30, bw, fonth+10);
		button[num]->copy_label(label[i]);
		button[num]->labelsize(B_FONT_SIZE);
		button[num]->align(FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
		button[num]->color(fl_rgb_color(216,216,216));
	}
	button[0]->shortcut(FL_Enter);

	icon = new YabAlertIcon(0,0,48,atxth+30+fonth+15, type);
	icon->redraw();

	win->color(fl_rgb_color(216,216,216));
	win->set_modal();
	win->resize(x, h/2-(atxth+30+fonth+15)/2, 310,atxth+30+fonth+15);
	win->show();
}

YabAlertWindow::~YabAlertWindow()
{
	delete icon;
	delete alertText;
	delete win;
}

Fl_Button* YabAlertWindow::GetButton(int i)
{
	if(i>=0 && i<3) return button[i];
	return NULL;
}

YabAlertIcon* YabAlertWindow::GetIcon()
{
	return icon;
}

