//CalendarControl.h

#ifndef CALENDARCONTROL_H
#define CALENDARCONTROL_H

#include <FL/Fl_Group.H> 

class CalendarControl: public Fl_Group	
{
	public: 
		CalendarControl(int x,
			 int y,
			 int day,
                  	 int month,
                  	 int year,
		 	 const char* type);
		CalendarControl(int x,
			 int y,
			 const char* date,
		 	 const char* type);
		~CalendarControl();
		//void draw();
		
		
		
	protected:
		void draw();
		
	private:
		static void CC_DateWindow(Fl_Widget *,  void *);
};	

#endif
