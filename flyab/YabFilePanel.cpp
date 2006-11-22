#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_File_Icon.H>
#include "YabFilePanel.h"
#include "YabInterface.h"
#include <stdio.h>

bool running = false;

YabFilePanel::YabFilePanel(int x, int y, int w, int h, const char* mode, const char* title, const char* directory, const char* filename)
{
	Fl_File_Icon::load_system_icons();
	result = "foobar";
	Fl::lock();
	win = new Fl_Double_Window(x, y, w, h, title);
	win->color(fl_rgb_color(216,216,216));
	win->begin();
		list = new Fl_File_Browser(10, 10, w-20, h-50);
		list->type(2);
		list->load(directory);
		list->iconsize(20);
		list->filter("*");

		btn_ok = new Fl_Button(w-100, h-30, 90, 20, "OK");
		btn_cancel = new Fl_Button(w-200, h-30, 90, 20, "Cancel");
	win->end();

	win->callback(FP_callback, (void *)"win");
//	list->callback(FP_callback, (void *)"list");
	list->callback(FP_callback);
	btn_ok->callback(FP_callback, (void *)"ok");
	btn_cancel->callback(FP_callback, (void *)"cancel");

	win->show();
	Fl::unlock();
}

YabFilePanel::~YabFilePanel()
{
	delete btn_ok, btn_cancel;
	delete list;
	delete win;
}

const char* YabFilePanel::Run()
{
	running = true;
	YabInterface *yi;
	while(running)
	{
		yi->Snooze(2.0);
	}
	Fl::lock();
	win->hide();
	Fl::unlock();
	return result;
}

void YabFilePanel::FP_callback(Fl_Widget *widget, void *data=0)
{
	if (Fl_File_Browser *b = dynamic_cast<Fl_File_Browser*>(widget))
	{
		printf("LISTE!!!\n%s\n", (char*)data);
	}
	if (char* d = static_cast<char*>(data))
	{
		if(d == "list")
		{
			return;
		}
		if(d == "ok")
		{
			printf("ok\n");
		}
		running = false;
	}
	return;
}


