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
	CC_Date date1(day,month,year);

	CC_Infos::CC_Infos *info = new CC_Infos;	
	info->posx_ = x;
	info->posy_ = y;
	
	info->d = date1.get_day();
	info->m = date1.get_month();
	info->y = date1.get_year();

	std::stringstream s;
	s << date1.get_day(); 
	s << ".";
	s << date1.get_month();   
	s << ".";
	s << date1.get_year(); 
	
	type = "MMDDYY."; //no function yet

	Fl_Input *input = new Fl_Input(x,y,65,20);
	Fl_Button *button = new Fl_Button(x+68,y,13,20);
	

	input->textsize(10);
	input->value(s.str().c_str());
	info->input_ = input;
	button->callback(CalendarControl::CC_DateWindow, (void *)info);
	
}

CalendarControl::CalendarControl(int x,
		   int y,
		   const char* date,
		   const char* type)
	 	:Fl_Widget(x,y,200,200)	
{
	//??
	int day,month,year;
	sscanf(date, "%2d.%2d.%4d", &day, &month, &year);
	
	std::cout << "TEST Constructer2: day month year " << day << " " << month << " " << year << endl;
	std::cout << "TEST Constructer2: x y "<< x << y << endl;

	//
	CC_Date date1(day,month,year);

	CC_Infos::CC_Infos *info = new CC_Infos;	
	info->posx_ = x;
	info->posy_ = y;
	
	info->d = date1.get_day();
	info->m = date1.get_month();
	info->y = date1.get_year();

	std::stringstream s;
	s << date1.get_day(); 
	s << ".";
	s << date1.get_month();   
	s << ".";
	s << date1.get_year(); 
	
	type = "MMDDYY."; //no function yet

	Fl_Input *input = new Fl_Input(x,y,65,20);
	Fl_Button *button = new Fl_Button(x+68,y,13,20);
	
	std::cout << "TEST Constructer2: input "<< input << endl;	
	
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

	CC_DrawView::DateWindowUpdate((void *)info);
}

void CalendarControl::draw() 
{
}
