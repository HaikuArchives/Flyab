#ifndef YABALERTICON_H
#define YABALERTICON_H

#include <FL/Fl_Image.H>
#include "imagedata.h"

class YabAlertIcon: public Fl_Widget
{
public:
	YabAlertIcon(int x, int y, int w, int h, const char* type);
	~YabAlertIcon();
	int HasType();
	void draw();
private:
	int x_, y_, w_, h_, type_;
	Fl_RGB_Image *myImage;
};

#endif
