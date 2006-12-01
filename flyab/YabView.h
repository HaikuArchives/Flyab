#ifndef YABVIEW_H
#define YABVIEW_H

#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <string>
#include <vector>
#include "YabWidget.h"

struct YabDrawing
{
        int command;
        int x1,y1,x2,y2,x3,y3,x4,y4;
        int r,g,b,alpha;
        const char* chardata;
		int p; // pattern
        // pattern p;
        // BBitmap *bitmap;
        // BFont font;
};

class YabView: public Fl_Group, public YabWidget
{
public:
	YabView(int x1, int y1, int width, int height, const char* id);
	~YabView();
	void draw();
	void SetColor(int r, int g, int b);
	void AddDrawing(YabDrawing* drawing);
	void FlushDrawings();
	void HasMenu(bool m);
	bool HasMenu();
private:
	std::vector<YabDrawing*> drawList;
	bool hasmenu;
	bool highSolid;
	Fl_Color bgcolor, lowcolor, highcolor;
	Fl_Box *bgbox;
};

#endif
