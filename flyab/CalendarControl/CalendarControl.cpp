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

//CC_Date ndd(0,0,00); 

using namespace std;

CalendarControl::CalendarControl(int x,
		   int y,
		   const char* date,
		   const char* type)
	 	:Fl_Group(x,y,200,200)	
{
	int day,month,year;
	sscanf(date, "%2d.%2d.%4d", &day, &month, &year);
	
	CC_Date date1(day,month,year);

	CC_Infos::CC_Infos *info = new CC_Infos;
	
	info->num = 0;
		
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
	
	redraw();
	end();

}

CalendarControl::~CalendarControl()
{
	//CC_Date date1(0,0,0);
}

void CalendarControl::CC_DateWindow(Fl_Widget *widget,  void *data)
{
	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos*)data;
	
	info->winx_ = widget->window()->x()+info->posx_;
	info->winy_ = widget->window()->y()+info->posy_+22;
	
	CC_DrawView::DateWindowUpdate((void *)info);
}

void CalendarControl::draw() 
{
	Fl_Group::draw();
}

void CalendarControl::test()
{
	std::cout << "TEST CC: OK" << std::endl;	
}
