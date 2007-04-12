//CC_DayView.cpp

#include <FL/Fl_Group.H> 
#include "CC_Date.cpp"

#ifndef CC_DAYVIEW_H
#define CC_DAYVIEW_H

class CC_DayView : public Fl_Group
{
	public:
     		CC_DayView(int x, int y, int w, int h); 
		~CC_DayView();
		void DeleteDayButtons();
	private:
		Fl_Button *dd[31]; //button day 1-31
		Fl_Group *cdg; //day area group
		 
     		
};
#endif

int g_day;
int g_month;
int g_year;

CC_DayView::CC_DayView(int x, int y, int w, int h): Fl_Group(x,y,w,h) 
{
	cdg = new Fl_Group(x,y,w,h);
	//cdg->box(FL_BORDER_BOX);
	cdg->box(FL_NO_BOX);
	cdg->begin();

	int x_ = x;
	cout << "hh " << g_month << endl;
	CC_Date::CC_Date date(g_day,g_month,g_year);
	
	switch (date.get_weekday(1))
	{
		case 0:
		 	x_=x_+(23*6);
			break;
		default:
			x_=24*(date.get_weekday(1)-1);
			break;
	}
 
	for(int i = 1;i <= date.month_days();i++){

		dd[i] = new Fl_Button(x_,y,20,10);
		dd[i]->box(FL_NO_BOX);
		dd[i]->labelsize(10);
	
		std::stringstream s;
		s << i;
		dd[i]->copy_label(s.str().c_str());	

		cdg->add(dd[i]);
		
		x_ = x_+23;
		//cout << "weekday in loop: " << i << ": "<< date.get_weekday(i) << endl;
		if(date.get_weekday(i) == 0){
			x_ = x; 
	        	y = y+17;
		}	
	}
	
	cdg->end();
}

CC_DayView::~CC_DayView()
{
}

// this is a a test workaround!!!
void CC_DayView::DeleteDayButtons()
{
	CC_Date::CC_Date date(g_day,g_month,g_year);

	for(int i = 1;i <= date.month_days()-1;i++){

		delete dd[5]; // not work 
		dd[i]->copy_label("");
		
		
	}
}
// end workaround

