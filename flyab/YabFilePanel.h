#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Browser.H>
#include "global.h"

class YabFilePanel
{
public:
	YabFilePanel(int x, int y, int w, int h, const char* mode, const char* title, const char* directory, const char* filename);
	~YabFilePanel();
	const char* Run();
private:
	Fl_Double_Window *win;
	Fl_Button *btn_ok, *btn_cancel;
	Fl_File_Browser *list;
	static void FP_callback(Fl_Widget *widget, void *data);
//	const char* result;
//	bool running;
};
