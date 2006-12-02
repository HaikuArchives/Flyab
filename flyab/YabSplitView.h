#ifndef YABSPLITVIEW_H
#define YABSPLITVIEW_H

#include <FL/Fl_Tile.H>
#include "YabView.h"
#include "YabWidget.h"
#include <string>
#include <vector>

using namespace std;

class YabSplitView: public Fl_Tile, public YabWidget
{
public:
	YabSplitView(int x_, int y_, int w_, int h_, const char* id, int mode_, int style_)
		:Fl_Tile(x_, y_, w_, h_), YabWidget(id)
	{
/*		viewid = id;
		mode = mode_;
		style = style_;
		x = x; y = y_;
		w = w_; h = h_;
*/
	}
	~YabSplitView()
	{
	}
/*
	YabView *AddView()
	{
		sprintf(num, "%d", views.size()+1);
		YabView *view = new YabView(x, y, w, h);
		views.push_back(view);
		return view;
	}

	draw()
	{
		for (int i=0; i<=views.size(); i++)
		{
			views(i)->draw();
		}
		Fl_Tile::draw();
	}

private:
	YabView *view1, *view2;
	vector<YabView*> views;
	string viewid;
	int mode, style;
*/
};
#endif
