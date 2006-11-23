#include <FL/Fl.H>
#include <FL/Fl_Button.H>
//#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include "global.h"
#include <stdlib.h>
#include "YabAlertWindow.h"
#include "YabAlertIcon.h"
#include "YabAlert.h"
#include "YabInterface.h"

int pressed = 0;
bool alert_running = false;

YabAlert::YabAlert(const char* text, const char* button1, const char* button2, const char* button3, const char* option)
{
	// if the first button is empty, we go with our standard - only first button and that one empty
	if (strcmp(button1, "")==0)
	{
		button1 = " ";
		button2 = NULL;
		button3 = NULL;
	}
	else
	// if we were provided with the first, we look, whether there are more
	{
		if (strcmp(button2, "")==0) button2 = NULL;
		if (strcmp(button3, "")==0) button3 = NULL;
	}
	// how much buttons do we have then?
	int b = (button2 != NULL) + (button3 != NULL) + 1;

	Fl::lock();
	alert_win = new YabAlertWindow(text, button1, button2, button3, option);
	alert_type = (alert_win->GetIcon())->HasType();

	// we always have at least one button
	// the (b-x): the rightmost button is drawn first and therefore returns the highest value
	Fl_Button *btn1 = alert_win->GetButton(0);
	btn1->callback(AlertCallback, (void *)(b-0));
	btn1->shortcut(FL_Enter);
	(btn1->window())->callback(AlertCallback, (void *)0);

	// we have to check for more buttons
	if (Fl_Button *btn2 = alert_win->GetButton(1))
		btn2->callback(AlertCallback, (void *)(b-1));
	if (Fl_Button *btn3 = alert_win->GetButton(2))
		btn3->callback(AlertCallback, (void *)(b-2));

	Fl::unlock();
}

YabAlert::~YabAlert()
{
	delete alert_win;
}

int YabAlert::HasType()
{
	return alert_type;
}

int YabAlert::ARun()
{
	// yeah, our alert blocks the hole program with a loop :)
	alert_running = true;
	YabInterface *yi;
	while (alert_running)
	{
		yi->Snooze(2.0);
	};
//	delete alert_win;
	return pressed;
}

void YabAlert::AlertCallback(Fl_Widget* widget, void *data)
{
	int msg = (int)data;
	Fl_Window *win;
	Fl::lock();
	if (! (win = dynamic_cast<Fl_Window*>(widget)))
		win = widget->window();

	win->hide();
	Fl::unlock();
	alert_running = false;
	pressed = msg;
	return;
}




