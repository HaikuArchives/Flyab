#ifndef YABALERTTEXT_H
#define YABALERTTEXT_H

#include <string>

class YabAlertText : public Fl_Widget
{
public:
	YabAlertText(int x, int y, int w, const char* text);
	int GetHeight();
	void draw();
private:
	std::string text_;
	int x_, y_, w_, h_;
};

#endif
