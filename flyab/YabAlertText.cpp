#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <iostream>
#include <string.h>

#include "YabAlertText.h"
#include "global.h"

using namespace std;

YabAlertText::YabAlertText(int x, int y, int w, const char* text)
	: Fl_Widget(x,y,w,120)
{
	int n = 1;
	int tw, th;
	string word, line, t;
	string txt = text;
	for (int i = 0; i < strlen(text)+1; i++)
	{
		if (txt[i] == 32 || txt[i] == 0)	// if one word is full
		{
			t = line + word;
			if (fl_width(t.c_str()) > w-30)
			{
				text_ = text_ + "\n ";
				n = n + 1;	// newline counter
				line = "";
			}
			else
			{
				text_ = text_ + " ";
				line = line + " ";
			}
			text_ = text_ + word;
			line = line + word;
			word = "";
		}
		else
			word = word + txt[i];
	}

	x_ = x;
	y_ = y;
	w_ = w;
	h_ = h_ * n;
	fl_measure(text_.c_str(), w_, h_);
	size(w_, h_);
}

YabAlertText::~YabAlertText()
{

}

int YabAlertText::GetHeight()
{
	return h_;
}

void YabAlertText::draw()
{
	fl_color(FL_BLACK);
	fl_font(0, B_FONT_SIZE);
	fl_draw(text_.c_str(), x_, y_, w_, h_, FL_ALIGN_LEFT);
}
