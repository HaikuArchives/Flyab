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
	YabSplitView(int x_, int y_, int w_, int h_, const char* id)
		:Fl_Tile(x_, y_, w_, h_), YabWidget(id)
	{
	}
	~YabSplitView()
	{
	}
};
#endif
