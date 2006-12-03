#ifndef YABSPLITVIEW_H
#define YABSPLITVIEW_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Tile.H>
#include "YabView.h"
#include "YabWidget.h"

class YabSplitView: public Fl_Tile, public YabWidget
{
public:
	YabSplitView(int x_, int y_, int w_, int h_, const char* id, int vert, int style);
	~YabSplitView();
	void SetPosition(int pos);
	int GetPosition();
	Fl_Group *GetGroup(int n);
	void SetMinimums(double, double);
//	int handle(int);

private:
	int vertical;
	double min1, min2;
	bool movement;
	Fl_Group *group1, *group2;
	YabView *view1, *view2;
	Fl_Box *resizebox;
};
#endif
