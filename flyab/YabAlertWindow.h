#ifndef YABALERTWIN_H
#define YABALERTWIN_H

#include <FL/Fl_Button.H>
#include "YabAlertIcon.h"
#include "YabAlertText.h"
#include "YabButton.h"

class YabAlertWindow 
{
public:
	YabAlertWindow(const char* text, const char* button1, const char* button2, const char* button3, const char* type);
	~YabAlertWindow();
	Fl_Button* GetButton(int i);
	YabAlertIcon* GetIcon();
private:
	Fl_Window *win;
	YabAlertIcon *icon;
	YabAlertText *alertText;
	Fl_Button *button[3];
};

#endif
