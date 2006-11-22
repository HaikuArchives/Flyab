#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <iostream>
#include <string>
#include "YabAlertIcon.h"
	
YabAlertIcon::YabAlertIcon(int x, int y, int w, int h, const char* type)
	: Fl_Widget(x,y,w,h)
{
	x_ = x;
	y_ = y;
	w_ = w;
	h_ = h;
	type_ = -1;
	myImage = NULL;

	std::string t = type;
	std::transform(t.begin(),t.end(),t.begin(),tolower);
	if(t.find("none",0) != std::string::npos)
		type_ = 0;
	else if(t.find("info",0) != std::string::npos)
	{
		type_ = 1;
		myImage = new Fl_RGB_Image(info, 48,48);
	}
	else if(t.find("idea",0) != std::string::npos)
	{
		type_ = 2;
		myImage = new Fl_RGB_Image(idea, 48,48);
	}
	else if(t.find("warning",0) != std::string::npos)
	{
		type_ = 3;
		myImage = new Fl_RGB_Image(warning, 48,48);
	}
	else if(t.find("stop",0) != std::string::npos)
	{
		type_ = 4;
		myImage = new Fl_RGB_Image(stop, 48,48);
	}
}

YabAlertIcon::~YabAlertIcon()
{
	delete myImage;
}

int YabAlertIcon::HasType()
{
	return type_;
}

void YabAlertIcon::draw()
{
	if(myImage)
	{
		myImage->draw(x_,y_);
		fl_rectf(0,48, 31,h()-48, 183,183,183);
	}

}
