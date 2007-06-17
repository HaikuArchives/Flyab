//CalendarControl.h

#ifndef CALENDARCONTROL_H
#define CALENDARCONTROL_H

#include <FL/Fl_Group.H> 
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include "CC_Infos.h"

class CalendarControl: public Fl_Group	
{
	public: 
		CalendarControl(int x,
			 int y,
			 const char* date,
		 	 const char* type);
		Fl_Input *input;
		Fl_Button *button;
		~CalendarControl();
		void test(Fl_Widget *,  void *);
		const char * GetDate();
		void SetDate(const char*);
		CC_Infos *info;
		
	protected:
		void draw();
		
	private:
		static void CC_DateWindow(Fl_Widget *,  void *);
};	

#endif
