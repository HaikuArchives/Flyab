//Test Application for calendar class

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>

#include <iostream>
#include <cstdlib> 
#include <ctime>

#include "CalendarControl.h"

using namespace std;


int main(int argc, char **argv) 
{
	Fl::scheme("gtk+");

	Fl_Window* window = new Fl_Window(100,100,400,300, "Calendar Test App!");
	
	CalendarControl *cl1 = new CalendarControl(20,20, 14,4,2007,"MMDDYY.");
	CalendarControl *cl2 = new CalendarControl(20,50, "10.11.1973","MMDDYY.");	
	
	//CalendarControl *cl2 = new CalendarControl(80,50, 11,2,2007,"MMDDYY.");	
	
	//
	
	//
	//window->redraw();
	window->end();
	window->show(argc, argv);

	return Fl::run();	
	
}
