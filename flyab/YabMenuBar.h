#ifndef YABMENUBAR_H
#define YABMENUBAR_H

#include <FL/Fl.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Widget.H>
#include <string>
#include <vector>

using namespace std;

string FilterString(const char* s)
{	// filter out some special chars
	string item = "";
	for (int i=0; i<strlen(s); i++)
	{
		if (s[i] == 38 || s[i] == 47 || s[i] == 95)
			item += "\\";
		item += s[i];
	}
	return item;
}

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
	void add(const char* menuhead, const char* menuitem, const char* submenu, const char* shortcut, void cb(Fl_Widget *w, void *d))
	{
		// submenu wanted?
		bool sub=false;
		if (strcmp(submenu, "") != 0) sub = true;

		// generate some kind of return code here
		string s = menuhead;
		s += ":";
		s += menuitem;
		if (sub)
		{
			s += ":";
			s += submenu;
		}
		ret.push_back(s);
		int n = ret.size()-1;

		// create a decent shortcut
		string sc;
		if (strcmp(shortcut, "") != 0) sc = "^";
		sc += shortcut;

		// filter out some special chars
		string item="";
		item += FilterString(menuhead);
		item += "/";

		if (sub)
		{
			item += FilterString(menuitem);
			item += "/";
			if (strcmp(submenu, "--") == 0)
			{
				item += "DasJott thanks jan__64 for his help";
				Fl_Menu_Bar::add(item.c_str(), sc.c_str(), cb, (void *)ret[n].c_str());
				int z = find_item(item.c_str())->size();
				z = size() - z - 1;
				if (z > 1) mode(z-1, FL_MENU_DIVIDER);
				remove(z);
			}
			else
			{
				item += FilterString(submenu);
				// finally add the entry
				Fl_Menu_Bar::add(item.c_str(), sc.c_str(), cb, (void *)ret[n].c_str());
			}
		}
		else
		{
			if (strcmp(menuitem, "--") == 0)
			{
				item += "DasJott thanks jan__64 for his help";
				Fl_Menu_Bar::add(item.c_str(), sc.c_str(), cb, (void *)ret[n].c_str());
				int z = find_item(item.c_str())->size();
				z = size() - z - 1;
				if (z > 1) mode(z-1, FL_MENU_DIVIDER);
				remove(z);
			}
			else
			{
				item += FilterString(menuitem);
				// finally add the entry
				Fl_Menu_Bar::add(item.c_str(), sc.c_str(), cb, (void *)ret[n].c_str());
			}
		}
	}
private:
	vector<string> ret;
};

#endif
