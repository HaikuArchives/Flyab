#include "YabSplitView.h"

YabSplitView::YabSplitView(int x, int y, int w, int h, const char* id, int vert, int style)
	:Fl_Tile(x, y, w, h), YabWidget(id)
{
	min1 = 0;
	min2 = 0;
	movement = true;
	vertical = vert;

	int x1, x2, y1, y2, w1, w2, h1, h2;
	if (vertical)
	{
		x1 = x; y1 = y;
		w1 = w/2; h1 = h;

		x2 = x+(w/2); y2 = y;
		w2 = w/2; h2 = h;
	}
	else
	{
		x1 = x; y1 = y;
		w1 = w; h1 = h/2;

		x2 = x; y2 = y+(h/2);
		w2 = w; h2 = h/2;
	}
	group1 = new Fl_Group(x1, y1, w1, h1);
	group2 = new Fl_Group(x2, y2, w2, h2);
	add(group1);
	add(group2);
	resizebox = new Fl_Box(x, y, w, h);
	resizable(resizebox);
}

YabSplitView::~YabSplitView()
{
}

Fl_Group* YabSplitView::GetGroup(int n)
{
	if (n == 1) return group1;
	if (n == 2) return group2;
	return NULL;
}

void YabSplitView::SetPosition(int pos)
{
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

		group1->size(group1->w(), pos);
		group2->resize(x2, y2, w2, h2);
	}
	group1->redraw();
	group2->redraw();
	return;
}
int YabSplitView::GetPosition()
{
	if (vertical)
		return group1->w();
	else
		return group1->h();
}
void YabSplitView::SetMinimums(double minimum1, double minimum2)
{
	min1 = minimum1;
	min2 = minimum2;

	if (vertical)
	{
//		resizebox->resize(x()+min1, y(), w()-x()-min1-min2, h());
	}
	else
	{
	}
}
/*
int YabSplitView::handle(int event)
{
	if (event == FL_DRAG)
	{
		if (group1->w() <= min1 || group2->w() <= min2)
			return 0;
		else
			return Fl_Tile::handle(event);
	}
	else
		return Fl_Tile::handle(event);
}
*/

