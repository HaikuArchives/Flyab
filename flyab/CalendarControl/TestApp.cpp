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
	
	CalendarControl *cl1 = new CalendarControl(10,20, "10.11.1973","MMDDYY.");	
	
	CalendarControl *cl2 = new CalendarControl(10,50, "11.1.1111","MMDDYY.");	
	cl2->SetDate("11.11.2007");
	std::cout << "TestApp GetDate = " << cl2->GetDate() << std::endl;	
	//

	//
	//window->redraw();
	window->end();
	window->show(argc, argv);

	return Fl::run();	
	
}
