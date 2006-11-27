#include <FL/Fl.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Widget.H>
#include <string>

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
	void Add(const char* menuhead, const char* menuitem, const char* shortcut, void cb(Fl_Widget *w, void *d))
	{
		// generate some kind of return code here
		return_path = menuhead;
		return_path += ":";
		return_path += menuitem;

		// create a decent shortcut
		std::string sc = "^";
		sc += shortcut;

		// filter out some special chars
		std::string item;
		for (int j=0; j<strlen(menuhead); j++)
		{
			if (menuitem[j] == 38 || menuhead[j] == 47 || menuhead[j] == 95)
				item += "\\";
			item += menuhead[j];
		}
		item += "/";
		for (int j=0; j<strlen(menuitem); j++)
		{
			if (menuitem[j] == 38 || menuhead[j] == 47 || menuhead[j] == 95)
				item += "\\";
			item += menuitem[j];
		}
		// finally add the entry
		Fl_Menu_Bar::add(item.c_str(), sc.c_str(), cb, (void *)item.c_str());
	}
	const char* GetPath()
	{
		return return_path.c_str();
	}
private:
	std::string return_path;
}
