#ifndef YABSPLITVIEW_H
#define YABSPLITVIEW_H

#include <FL/Fl_Group.H>
#include <FL/Fl_Tile.H>
#include "YabWidget.h"

using namespace std;

class YabSplitView: public Fl_Tile, public YabWidget
{
public:
	YabSplitView(int x_, int y_, int w_, int h_, const char* id, int vert);
	~YabSplitView();
	void SetPosition(int pos);
	int GetPosition();

private:
	int x, y, w, h, vertical;

};
#endif
