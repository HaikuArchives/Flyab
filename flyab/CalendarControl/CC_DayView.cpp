//CC_DayView.cpp

#include <FL/Fl_Group.H> 
#include "CC_Date.cpp"

#ifndef CC_DAYVIEW_H
#define CC_DAYVIEW_H

class CC_DayView : public Fl_Group
{
	public:
     		CC_DayView(int x, int y, int w, int h, void *); 
		~CC_DayView();
		static void SelectDay(Fl_Widget *, void *);
	private:
		Fl_Button *dd[31+1]; //button day 1-31 //bug fix 31 to 31+1 (32) set, else by 31 month 1,3,10,12 chrashed!!!
		Fl_Group *cdg; //day area group
		
		 
     		
};
#endif

CC_DayView::CC_DayView(int x, int y, int w, int h,void *data): Fl_Group(x,y,w,h) 
{
	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos*)data;
	//cout << "extern in CC_DayView::CC_DayView: " << info->posx_ << " : " << info->posy_ << ":" << info->mb_  << endl;
	//cout << "extern in CC_DayView::CC_DayView: " << info->d << " : " << info->m << " : " << info->y << endl;


	cdg = new Fl_Group(x,y,w,h);
	//cdg->box(FL_BORDER_BOX);
	cdg->box(FL_NO_BOX);
	cdg->begin();

	int x_ = x;
	cout << "hh " << info->m << endl;
	CC_Date::CC_Date date(info->d,info->m,info->y);
	
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
		dd[i]->callback(CC_DayView::SelectDay, (void *)info);

		if(i == date.get_day()){
			dd[i]->box(FL_BORDER_BOX);
		}
	
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

void CC_DayView::SelectDay(Fl_Widget *widget,void *data)
{	
	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos*)data;
	const char* b_lab = widget->label();
	
	std::cout << "Test: Button " << b_lab << " is select!" << endl;
	
	std::stringstream s;
	s << b_lab; 
	s << ".";
	s << info->m;  
	s << ".";
	s << info->y;
	
	info->input_->value(s.str().c_str());	
	info->d = atoi(b_lab);

	for(int i = 0;i <= info->num-1;i++){
		info->oldwin[i]->Fl_Window::~Fl_Window(); 
	}
	info->num = 0;
	info->win_->Fl_Window::~Fl_Window(); 
	
}


