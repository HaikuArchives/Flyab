#ifndef YABMENUBAR_H
#define YABMENUBAR_H

#include <FL/Fl.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Widget.H>
#include <string>
#include <vector>

using namespace std;

class YabMenuBar: public Fl_Menu_Bar
{
public:
	YabMenuBar(int w, int h)
		: Fl_Menu_Bar(0, 0, w, h)
	{
		box(FL_THIN_UP_BOX);
		labelsize(B_FONT_SIZE);
		color(fl_rgb_color(B_GREY));
	}
	~YabMenuBar()
	{
	}
	void add(const char* menuhead, const char* menuitem, const char* shortcut, void cb(Fl_Widget *w, void *d))
	{
		// generate some kind of return code here
		string s = menuhead;
		s += ":";
		s += menuitem;
		ret.push_back(s);
		int n = ret.size()-1;

		// create a decent shortcut
		string sc = "^";
		sc += shortcut;

		// filter out some special chars
		string item;
		for (int j=0; j<strlen(menuhead); j++)
		{
			if (menuhead[j] == 38 || menuhead[j] == 47 || menuhead[j] == 95)
				item += "\\";
			item += menuhead[j];
		}
		item += "/";
		if (strcmp(menuitem, "--") == 0)
			Fl_Menu_Bar::add(item.c_str(), "", cb, (void *)"", FL_MENU_DIVIDER);
		else
		{
			for (int j=0; j<strlen(menuitem); j++)
			{
				if (menuitem[j] == 38 || menuitem[j] == 47 || menuitem[j] == 95)
					item += "\\";
				item += menuitem[j];
			}
			// finally add the entry
			Fl_Menu_Bar::add(item.c_str(), sc.c_str(), cb, (void *)ret[n].c_str());
		}
	}
private:
	vector<string> ret;
};

#endif
