//CC_DrawView.cpp

#include <FL/Fl_Group.H> 

#ifndef CC_DRAWVIEW_H
#define CC_DRAWVIEW_H

static const char *MonthName[12] = {
    "Januar","Februar","Maerz","April","Mai","Juni","Juli","August","September","Oktober","November","Dezember"
};

const unsigned char umlaut_ae = static_cast<unsigned char>(228);

static const char *DayName[7] = {
    "Mo","Di","Mi","Do","Fr","Sa","So"
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
	dvb->color(fl_rgb_color(216,216,216)); 

	mb = new Fl_Box(5,3,90,20);
	mb->box(FL_NO_BOX);
	mb->labelsize(10);
	//info->mb_ = mb;

	yb = new Fl_Box(85,3,95,20);
	yb->box(FL_NO_BOX);
	yb->labelsize(10);
	//info->yb_ = yb;
	
	Fl_Button *Mbutdo = new Fl_Button(2,(int)2.5,20,20,"<<");
	Mbutdo->box(FL_NO_BOX);
	Mbutdo->labelsize(10);
	Mbutdo->callback(month_down, (void *)info);	

	Fl_Button *Mbutup = new Fl_Button(75,(int)2.5,20,20,">>");
	Mbutup->box(FL_NO_BOX);
	Mbutup->labelsize(10);
	Mbutup->callback(month_up,(void *) info);

	Fl_Button *Ybutdo = new Fl_Button(95,(int)2.5,20,20,"<<");
	Ybutdo->box(FL_NO_BOX);
	Ybutdo->labelsize(10);
	Ybutdo->callback(year_down, (void *)info);

     	Fl_Button *Ybutup = new Fl_Button(148,(int)2.5,20,20,">>");
	Ybutup->box(FL_NO_BOX);
	Ybutup->labelsize(10);
	Ybutup->callback(year_up, (void *)info);
		
	Fl_Button *d[6];  
	int x_ = 5;
	for(int i = 0;i <= 6;i++){

		d[i] = new Fl_Button(x_,25,18,18);
                d[i]->box(FL_NO_BOX);
                d[i]->labelsize(10); 
		d[i]->copy_label(DayName[i]);
                               
		x_ = x_+23;
	}
		
	dayview = new CC_DayView(3,45,163,95, (void *)info);
 	
	Fl_Box *cdb = new Fl_Box(5,148,160,20);
	cdb->box(FL_NO_BOX);
	cdb->labelsize(10);

	/*
	struct tm *tmnow;
	time_t tnow;
	time(&tnow);
	tmnow = localtime(&tnow);
	*/
	CC_Date rdd(0,0,00);
	int rd = rdd.CC_Date::get_realdate(1);
	int rm = rdd.CC_Date::get_realdate(2);
	int ry = rdd.CC_Date::get_realdate(3);
	

	std::stringstream s;
	s << rd;
	s << " ";
	s << MonthName[rm-1]; 
	s << " ";
	s << ry;
	cdb->copy_label(s.str().c_str());
	end();
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

	info->y = info->y+1;

	DateWindowUpdate((void *)info);
	
}

void CC_DrawView::year_down(Fl_Widget *widget, void *data)
{
	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos *)data;

	info->y = info->y-1;

	DateWindowUpdate((void *)info);	
}

void CC_DrawView::month_up(Fl_Widget *widget, void *data)
{
	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos *)data;

	if(info->m >= 12){
		info->m = 1;
	}
	else {
		info->m = info->m+1;
	}

	DateWindowUpdate((void *)info); 	
}

void CC_DrawView::month_down(Fl_Widget *widget, void *data)
{
	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos *)data;

	if(info->m == 1){
		info->m = 12;
	}
	else {
		info->m = info->m-1;
	}

	DateWindowUpdate((void *)info);
}

void CC_DrawView::DateWindowUpdate(void *data)
{
	CC_Infos::CC_Infos *info = (CC_Infos::CC_Infos *)data;

	//std::cout << "Test: " << "open old windows: "<< info->num << "\n";
	
	info->oldwin[info->num] = info->win_;
	
	//When used Calendar in Tab-,Spit- ore Boxview froze the yab, so i make a dirty Workaround.
	//Bugfix_Workaround from DateWindowUpdate when used Calendar in Tab-,Spit- ore Boxview. 
	if(info->num == 0) {
		Fl::lock();
		
		Fl_Window::Fl_Window* dumy_win = new Fl_Window(0,0,0,0, ""); //a dumy window
		dumy_win->end();
		dumy_win->show();
		dumy_win->redraw();
		dumy_win->hide();
		info->dumy_win_ = dumy_win;
		
		Fl_Window::Fl_Window* win = new Fl_Window(info->winx_,info->winy_,170,170, "");//a true window
		win->border(0); 
		win->set_modal();
	
		info->win_ = win;
	

		CC_DrawView *dv = new CC_DrawView(0,0,170,170, (void *)info); 
		
		std::stringstream s_month;
		if(info->m == 3){
			s_month << "M" << umlaut_ae << "rz";
		}
		else{
		s_month << MonthName[info->m-1];
		}

		dv->mb->copy_label(s_month.str().c_str());

		std::stringstream s_year;
		s_year << info->y;
		dv->yb->copy_label(s_year.str().c_str());
		dv->end();


		win->add(dv);
		win->redraw();
		win->end();
		win->show();


		info->num = info->num+1;
	
		Fl::unlock();
		return;
	}
	//Workaround end and add a ture DateWindowUpdate
	else {

		Fl::lock();
		Fl_Window::Fl_Window* win = new Fl_Window(info->winx_,info->winy_,170,170, "");
		win->border(0);
		win->set_modal();
	
		info->win_ = win;
	
		CC_DrawView *dv = new CC_DrawView(0,0,170,170, (void *)info); 
		
		std::stringstream s_month;
		if(info->m == 3){
			s_month << "M" << umlaut_ae << "rz";
		}
		else{
			s_month << MonthName[info->m-1];
		}
		dv->mb->copy_label(s_month.str().c_str());

		std::stringstream s_year;
		s_year << info->y;
		dv->yb->copy_label(s_year.str().c_str());
		dv->end();

		win->add(dv);
		win->redraw();
		win->end();
		win->show();

		info->num = info->num+1;
	
		Fl::unlock();
		return;
	}	
}

