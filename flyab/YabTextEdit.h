#ifndef YABTEXTEDIT_H
#define YABTEXTEDIT_H

#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Buffer.H>
#include "YabWidget.h"

class YabTextEdit: public Fl_Text_Editor, public YabWidget
{
public:
	YabTextEdit(int x, int y, int w, int h, const char* id)
		: Fl_Text_Editor(x,y,w,h), YabWidget(id)
	{
		b = new Fl_Text_Buffer();
		buffer(b);
	}

	~YabTextEdit()
	{
		delete b;
	}

private:
	Fl_Text_Buffer *b;

};

#endif
