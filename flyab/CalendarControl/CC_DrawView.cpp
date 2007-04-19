//CC_DrawView.cpp

#include <FL/Fl_Group.H> 

#ifndef CC_DRAWVIEW_H
#define CC_DRAWVIEW_H

class CC_DrawView : public Fl_Group
{
	public:
     		CC_DrawView(int x, int y, int w, int h, void *); 
     		void draw();
		static void month_up(Fl_Widget *, void *);
		static void month_down(Fl_Widget *, void *);
		static void year_up(Fl_Widget *, void *);
		static void year_down(Fl_Widget *, void *);
		static void test();
		Fl_Box *mb; //month box
		Fl_Box *yb; //year box
		
	private:
		Fl_Box *dbt; //day titel box
		CC_DayView::CC_DayView *dayview; //obj dayview
		
		
		
};
#endif

static const char *MonthName[12] = {
    "Januar","Februar","Maerz","April","Mai","Juni","Juli","August","September","Oktober","November","Dezember"
};

CC_DrawView::CC_DrawView(int x, int y, int w, int h, void *data) : Fl_Group(x,y,w,h) 
{
	//test(); // OK
	
	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos*)data;
	cout << "extern in CC_DrawView::CC_DrawView: " << info->posx_ << " : " << info->posy_ << ":" << info->mb_  << endl;
	cout << "extern in CC_DrawView::CC_DrawView: " << info->d << " : " << info->m << " : " << info->y << endl;

	Fl_Box *dv = new Fl_Box(0,0,w,h);
	dv->box(FL_UP_BOX);

	mb = new Fl_Box(5,3,90,20);
	mb->box(FL_NO_BOX);
	mb->labelsize(10);
	info->mb_ = mb;
	

	yb = new Fl_Box(85,3,95,20);
	yb->box(FL_NO_BOX);
	yb->labelsize(10);
	info->yb_ = yb;
	
	dbt = new Fl_Box(5,25,160,20);
	dbt->box(FL_NO_BOX);
	dbt->labelsize(10);
	dbt->label("Mo    Di    Mi    Do    Fr    Sa    So");

	Fl_Button *Mbutdo = new Fl_Button(2,(int)2.5,20,20,"<<");
	Mbutdo->box(FL_NO_BOX);
	Mbutdo->callback(month_down, (void *)info);	

	Fl_Button *Mbutup = new Fl_Button(75,(int)2.5,20,20,">>");
	Mbutup->box(FL_NO_BOX);
	Mbutup->callback(month_up,(void *) info);

	Fl_Button *Ybutdo = new Fl_Button(95,(int)2.5,20,20,"<<");
	Ybutdo->box(FL_NO_BOX);
	Ybutdo->callback(year_down, (void *)info);

     	Fl_Button *Ybutup = new Fl_Button(148,(int)2.5,20,20,">>");
	Ybutup->box(FL_NO_BOX);
	Ybutup->callback(year_up, (void *)info);
				
	

	dayview = new CC_DayView(3,45,163,95, (void *)info);
	//dayview->DeleteDayButtons((void *)info); //OK

 	
	Fl_Box *cdb = new Fl_Box(5,148,160,20);
	cdb->box(FL_NO_BOX);
	cdb->labelsize(10);


	struct tm *tmnow;
	time_t tnow;
	time(&tnow);
	tmnow = localtime(&tnow);
	
	int rd = tmnow->tm_mday;
	int rm = tmnow->tm_mon + 1;
	int ry = tmnow->tm_year + 1900;
	
	std::stringstream s;
	s << rd;
	s << " ";
	s << MonthName[rm-1]; 
	s << " ";
	s << ry;
	cdb->copy_label(s.str().c_str());
}

void CC_DrawView::test()
{
	cout << "TEST" << endl;
}

void CC_DrawView::draw()
{
	Fl_Group::draw();
        fl_color(170,170,170);
	fl_line(3, 25, 165, 25);
	fl_line(3, 145, 165, 145);
}

void CC_DrawView::year_up(Fl_Widget *widget, void *data)
{
	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos *)data;
	std::cout << "OK year_up | new label" << "\n";
	

	std::stringstream t;
	t << info->y+1;
	info->yb_->copy_label(t.str().c_str());
	info->y = info->y+1;
	
	//std::cout << "Das Jahr ist nun: " << g_year << "\n";
	CC_Date::CC_Date date(info->d,info->m,info->y);
	//date.ausgabe;
	//CC_DayView::DeleteDayButtons((void *)info); //error

	//TODO: delate dayview and make new dayview with new month/year etc...
	
}

void CC_DrawView::year_down(Fl_Widget *widget, void *data)
{
	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos *)data;
	std::cout << "OK year_down" << "\n";
	
	std::stringstream t;
	t << info->y-1;
	info->yb_->copy_label(t.str().c_str());
	info->y = info->y-1;

	//TODO: delate dayview and make new dayview with new month/year etc...
	
}

void CC_DrawView::month_up(Fl_Widget *widget, void *data)
{
	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos *)data;
	std::cout << "month_up" << "\n";


	std::stringstream t;

	if(info->m >= 12){
		info->m = 1;
		t << MonthName[info->m-1];
		info->mb_->copy_label(t.str().c_str());
	}
	else {
		t << MonthName[(info->m-1)+1];
		info->mb_->copy_label(t.str().c_str());
		info->m = info->m+1;
		
		//TODO: delate dayview and make new dayview with new month/year etc...
		
	} 
	
}

void CC_DrawView::month_down(Fl_Widget *widget, void *data)
{
	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos *)data;
	std::cout << "month_down" << "\n";

	std::stringstream t;

	if(info->m == 1){
		info->m = 12;
		t << MonthName[info->m-1];
		info->mb_->copy_label(t.str().c_str());
		return;
	}
	else {
		t << MonthName[(info->m-1)-1];
		info->mb_->copy_label(t.str().c_str());
		info->m = info->m-1;
		
		//TODO: delate dayview and make new dayview with new month/year etc...
	}
}
