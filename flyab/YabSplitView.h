#ifndef YABSPLITVIEW_H
#define YABSPLITVIEW_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Tile.H>
#include "YabView.h"
#include "YabWidget.h"
#include <string>
#include <vector>

using namespace std;

class YabSplitView: public Fl_Tile, public YabWidget
{
public:
	YabSplitView(int x_, int y_, int w_, int h_, const char* id, int vert)
		:Fl_Tile(x_, y_, w_, h_), YabWidget(id)
	{
		vertical = vert;
	}
	~YabSplitView()
	{
	}
	void SetPosition(int pos)
	{
		Fl_Group *group1 = dynamic_cast<Fl_Group*>(child(0));
		Fl_Group *group2 = dynamic_cast<Fl_Group*>(child(1));
		if (!group1 || !group2) return;

		if (vertical)
		{
			int dif_w = group1->w() - pos;
			int x2 = group2->x() - dif_w;
			int y2 = group2->y();
			int w2 = group2->w() + dif_w;
			int h2 = group2->h();

			group1->size(pos, group1->h());
			group2->resize(x2, y2, w2, h2);
		}
		else
		{
			int dif_h = group1->h() - pos;
			int x2 = group2->x();
			int y2 = group2->y() - dif_h;
			int w2 = group2->w();
			int h2 = group2->h() + dif_h;

			group1->size(pos, group1->h());
			group2->resize(x2, y2, w2, h2);
		}
		group1->redraw();
		group2->redraw();
		return;
	}
	int GetPosition()
	{
		if (Fl_Group *group1 = dynamic_cast<Fl_Group*>(child(0)))
		{
			if (vertical)
				return group1->w();
			else
				return group1->h(); 
		}
	}
private:
	int x, y, w, h, vertical;

};
#endif
