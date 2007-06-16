#ifndef YABCOLUMNBOX_H
#define YABCOLUMNBOX_H

#include <vector>
#include <string>
#include <iostream>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Table_Row.H>
#include "global.h"
#include "YabWidget.h"

class YabColumnBox: public Fl_Table_Row, public YabWidget
{
private:
	bool isResizable;
	std::vector<std::vector<std::string> > myrows;
	int columnCount;
	std::vector<Fl_Align> alignments;

public:
	YabColumnBox(int x1, int y1, int width, int height, const char* id, bool hasScrollbar, bool resizable)
		: Fl_Table_Row(x1,y1,width,height), YabWidget(id)
	{
		color(fl_rgb_color(B_GREY));
		isResizable = resizable;
		type(Fl_Table_Row::SELECT_SINGLE);
		columnCount = 0;
		std::vector<std::string> mycolumns;
		myrows.push_back(mycolumns);
	}

	void AddColumn(const char* item, int pos, Fl_Align align);
	void AddItem(const char* item, int rowpos, int colpos);

protected:
	void draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H);
};

#endif
