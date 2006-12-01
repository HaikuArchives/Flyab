#ifndef YABSTACKVIEW_H
#define YABSTACKVIEW_H

#include <FL/Fl_Wizard.H>
#include "YabView.h"
#include "YabWidget.h"
#include "global.h"
#include <string>
#include <vector>

class YabStackView: public Fl_Wizard, public YabWidget
{
public:
	YabStackView(int x_, int y_, int w_, int h_, const char* id)
		:Fl_Wizard(x_, y_, w_, h_), YabWidget(id)
	{
		x=x_; y=y_;
		w=w_; h=h_;
		viewcounter = 0;
		viewname = id;
		color(fl_rgb_color(B_GREY));
		selection_color(fl_rgb_color(B_GREY));
		box(FL_NO_BOX);
	}
	~YabStackView()
	{
	}
	YabView *NewView()
	{
		char num[3];
		viewcounter++;
		sprintf(num, "%d", viewcounter);
		std::string newname = viewname;
		newname += num;

		YabView *view = new YabView(x, y, w, h, newname.c_str());
		view->end();
		stackview.push_back(view);

		add(view);
		redraw();
		return view;
	}

	void Set(int num)
	{
		num--;
		if (num > viewcounter) num = viewcounter;
		if (num < 0) num = 0;
		value(stackview[num]);
	}

	int Get()
	{
		int num;
		for (num=0; num<= stackview.size(); num++)
			if (stackview[num] == value())
				break;
		num++;
		return num;
	}

private:
	int x, y, w, h, viewcounter;
	std::string viewname;
	std::vector<YabView*> stackview;
};
#endif
