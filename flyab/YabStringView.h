#ifndef YABSTRINGVIEW_H
#define YABSTRINGVIEW_H

#include <FL/Fl_Box.H>
#include <string>
#include "YabWidget.h"
#include "global.h"

class YabStringView: public Fl_Box, public YabWidget
{
public:
	YabStringView(int x, int y, int width, int height, const char* id, const char* text_)
		: Fl_Box(x,y,width,height), YabWidget(id)
	{
		text = text_;

	}

private:
	const char* text;

};

#endif
