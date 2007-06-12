#ifndef YABTABVIEW_H
#define YABTABVIEW_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Tabs.H>
#include "YabView.h"
#include "YabWidget.h"
#include "global.h"
#include <string>
#include <vector>

class YabTabView: public Fl_Tabs, public YabWidget
{
public:
	YabTabView(int x_, int y_, int w_, int h_, const char* id, int bottomtabs)
		:Fl_Tabs(x_, y_, w_, h_), YabWidget(id)
	{

		box(FL_UP_BOX);
		labelsize(B_FONT_SIZE);
		color(fl_rgb_color(B_GREY));
		selection_color(fl_rgb_color(B_GREY));

		tabheight = (int)labelsize() + 10;
		x=x_; y=y_;
		w=w_; h=h_-tabheight;
		if (bottomtabs != 1) y=y_+tabheight;
		viewname = id;
		viewcounter = 0;
		end();
		show();
	}

	YabView *NewTab(const char* label)
	{
		char num[3];
		viewcounter++;
		sprintf(num, "%d", viewcounter);
		std::string newname = viewname;
		newname += num;
		
		
		// workaround for a bug in Fl_Tabs
		Fl_Group *box = new Fl_Group(x, y, w, h);
		box->box(FL_UP_FRAME);
		box->labelsize(B_FONT_SIZE);
		box->copy_label(label);

		YabView *view = new YabView(x+2, y+2, w-4, h-4, newname.c_str());
		view->end();
		tabview.push_back(view);

		box->add(view);
		add(box);
		redraw();
		end();

		return view;
	}

	void Set(int num)
	{
		num--;
		if (num > viewcounter) num = viewcounter;
		if (num < 0) num = 0;
		value(tabview[num]);
	}

	int Get()
	{
		int num;
		for (num=0; num<= tabview.size(); num++)
			if (tabview[num] == value())
				break;
		num++;
		return num;
	}

	int Height()
	{
		return tabheight;
	}
	
	
	~YabTabView()
	{
	}


private:
	int x, y, w, h, viewcounter, tabheight;
	std::string viewname;
	std::vector<YabView*> tabview;
};
#endif
