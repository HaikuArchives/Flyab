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
	Fl_Color selectionTextColor, textColor, rowDividerColor, backgroundColor;

public:
	YabColumnBox(int x1, int y1, int width, int height, const char* id, bool hasScrollbar, bool resizable)
		: Fl_Table_Row(x1,y1,width,height), YabWidget(id)
	{
		selection_color(fl_rgb_color(195,195,195));
		selectionTextColor = fl_rgb_color(0,0,0);
		textColor = fl_rgb_color(0,0,0);
		rowDividerColor = fl_rgb_color(195,195,195);
		backgroundColor = fl_rgb_color(255,255,255);
		isResizable = resizable;
		type(Fl_Table_Row::SELECT_SINGLE);
		columnCount = 0;
		std::vector<std::string> mycolumns;
		myrows.push_back(mycolumns);
	}

	void AddColumn(const char* item, int pos, Fl_Align align);
	//void AddItem(const char* item, int rowpos, int colpos);
	void AddItem(const char* item, int colpos, int rowpos);
	void RemoveAll();
	const char* ItemAt(int column, int position);
	int SelectedRow();
	int NumRows();
	int NumColumns();
	void SetColor(int color, int r, int g, int b);

protected:
	void draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H);
};

#endif
