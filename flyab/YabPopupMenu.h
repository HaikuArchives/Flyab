#ifndef YABPOPUPMENU_H
#define YABPOPUPMENU_H

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Pack.H>
#include <string>
#include <vector>

using namespace std;

class YabPopupMenu: public Fl_Double_Window
{
public:
	YabPopupMenu(int x, int y, const char* contents);
	~YabPopupMenu();
	int value();
	const char* text(int n);
	int Run();
	void draw();

private:
	int handle(int);

	bool running;
	int w, h, line_h, line_w, clicked_item, selected_item;
	Fl_Color textcolor_low, textcolor_high, bgcolor_low, bgcolor_high;
	Fl_Pack *list;
	vector<Fl_Box*> itembox;
	vector<string> items;

};

#endif
