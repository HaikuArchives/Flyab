//CalendarControl.cpp

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H> 
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>   
#include <FL/fl_draw.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>

#include <cstdlib> 
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <sstream>

#include "CalendarControl.h"
#include "CC_Infos.h"
#include "CC_DayView.cpp"
#include "CC_DrawView.cpp"

using namespace std;

CalendarControl::CalendarControl(int x,
		   int y,
		   int day,
                   int month,
                   int year,
		   const char* type)
	 	:Fl_Widget(x,y,200,200)	
{
	CC_Date date(day,month,year);

	CC_Infos::CC_Infos *info = new CC_Infos;	
	info->posx_ = x;
	info->posy_ = y;
	
	info->d = date.get_day();
	info->m = date.get_month();
	info->y = date.get_year();

	std::stringstream s;
	s << date.get_day(); s_day << date.get_day();
	s << ".";
	s << date.get_month(); s_month << MonthName[date.get_month()-1];  
	s << ".";
	s << date.get_year(); s_year << date.get_year();
	
	type = "MMDDYY."; //no function yet

	Fl_Input *input = new Fl_Input(x,y,65,20);
	Fl_Button *button = new Fl_Button(x+68,y,13,20);

	input->textsize(10);
	input->value(s.str().c_str());
	info->input_ = input;
	button->callback(CalendarControl::CC_DateWindow, (void *)info);
	
}

CalendarControl::~CalendarControl()
{
}

void CalendarControl::CC_DateWindow(Fl_Widget *widget,  void *data)
{

	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos*)data;

	int winx = widget->window()->x()+info->posx_;
	int winy = widget->window()->y()+info->posy_+22;
	widget->redraw();
	
	info->winx_ = winx;
	info->winy_ = winy;
	info->num = info->num-1;

	CC_DrawView::DateWindowUpdate((void *)info);
}

void CalendarControl::draw() 
{
}
