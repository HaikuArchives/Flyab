#ifndef YABCALENDAR_H
#define YABCALENDAR_H

#include "CalendarControl/CalendarControl.h"
#include "global.h"
#include <string>
#include "YabWidget.h"


class YabCalendar: public CalendarControl, public YabWidget
{
public:
	YabCalendar(double x, double y, const char* id, const char* format, const char* date)
		: CalendarControl((int)x,(int)y,date,format), YabWidget(id)
	{
		//	
		//std::cout << "test YABCALENDAR_H: " << x << ":" << y << std::endl;
		
	}
	
};

#endif
