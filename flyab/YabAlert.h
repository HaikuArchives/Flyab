#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include "YabAlertWindow.h"
#include "global.h"

class YabAlert
{
public:
	YabAlert(const char* text, const char* button1, const char* button2, const char* button3, const char* option);
	~YabAlert();
	int ARun();
	int HasType();
private:
	YabAlertWindow *alert_win;
	int alert_type;
	static void AlertCallback(Fl_Widget* widget, void *data);
};

