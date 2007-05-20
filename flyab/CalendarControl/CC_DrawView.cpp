//CC_DrawView.cpp

#include <FL/Fl_Group.H> 

#ifndef CC_DRAWVIEW_H
#define CC_DRAWVIEW_H

using namespace std;

namespace Umlaut
{
	const unsigned char AE = static_cast<unsigned char>(196);
        const unsigned char ae = static_cast<unsigned char>(228);
        const unsigned char OE = static_cast<unsigned char>(214);
        const unsigned char oe = static_cast<unsigned char>(246);
        const unsigned char UE = static_cast<unsigned char>(220);
        const unsigned char ue = static_cast<unsigned char>(252);
        const unsigned char ss = static_cast<unsigned char>(223);
}

static const char *MonthName[12] = {
    "Januar","Februar","Maerz","April","Mai","Juni","Juli","August","September","Oktober","November","Dezember"
};

class CC_DrawView : public Fl_Group
{
	public:
     		CC_DrawView(int x, int y, int w, int h, void *); 
     		void draw();
		static void month_up(Fl_Widget *, void *);
		static void month_down(Fl_Widget *, void *);
		static void year_up(Fl_Widget *, void *);
		static void year_down(Fl_Widget *, void *);
		static void DateWindowUpdate(void *);
		Fl_Box *mb; //month box
		Fl_Box *yb; //year box
	private:
		Fl_Box *dbt; //day titel box
		CC_DayView::CC_DayView *dayview; //obj dayview		
};
#endif

CC_DrawView::CC_DrawView(int x, int y, int w, int h, void *data) : Fl_Group(x,y,w,h) 
{
	
	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos*)data;

	Fl_Box *dvb = new Fl_Box(0,0,w,h);
	dvb->box(FL_UP_BOX);

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
	//std::cout << "OK year_up | new label" << "\n";
	
	std::stringstream t;
	t << info->y+1;
	info->yb_->copy_label(t.str().c_str());

	info->y = info->y+1;

	DateWindowUpdate((void *)info);
	
}

void CC_DrawView::year_down(Fl_Widget *widget, void *data)
{
	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos *)data;
	
	std::stringstream t;
	t << info->y-1;
	info->yb_->copy_label(t.str().c_str());
	info->y = info->y-1;

	DateWindowUpdate((void *)info);	
}

void CC_DrawView::month_up(Fl_Widget *widget, void *data)
{
	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos *)data;

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
		
		DateWindowUpdate((void *)info);		
	} 	
}

void CC_DrawView::month_down(Fl_Widget *widget, void *data)
{
	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos *)data;

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
		
		DateWindowUpdate((void *)info);
	}
}

void CC_DrawView::DateWindowUpdate(void *data)
{
	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos *)data;

	//update dates windows

	//std::cout << "Test: " << "open old windows: "<< info->num << "\n";
	
	info->oldwin[info->num] = info->win_;
	
	Fl::lock();
		
	Fl_Window* win = new Fl_Window(info->winx_,info->winy_,170,170, "dateview");
	win->border(0); //0 = no border
	win->set_modal();
	info->win_ = win;
	
	CC_DrawView *dv = new CC_DrawView(0,0,170,170, (void *)info); //CC_DrawView.cpp
		
	std::stringstream s_month;
	if(info->m == 3){
		s_month << "M" << Umlaut::ae << "rz";
	}
	else{
		s_month << MonthName[info->m-1];
	}
	dv->mb->copy_label(s_month.str().c_str());

	std::stringstream s_year;
	s_year << info->y;
	dv->yb->copy_label(s_year.str().c_str());
	
	win->add(dv);
	win->end();
	win->show();

	info->num = info->num+1;

	Fl::unlock();
	return;	
}
