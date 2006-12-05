#include <FL/Fl.H>
#include "YabCheckboxImage.h"

YabCheckboxImage::YabCheckboxImage(int x, int y, const char* id, const char* en_on, const char* en_off, const char* dis_on, const char* dis_off, int is_on)
	:Fl_Wizard(x, y, 10, 10), YabWidget(id)
{
	box(FL_NO_BOX);
	is_clicked = false;
	is_checked = false;

	pic_ennormal = new Fl_PNG_Image(en_off);
	pic_enclicked = new Fl_PNG_Image(en_on);
	pic_disnormal = new Fl_PNG_Image(dis_off);
	pic_disclicked = new Fl_PNG_Image(dis_on);

	int w = pic_ennormal->w();
	int h = pic_ennormal->h();

	box_normal = new Fl_Box(x, y, w, h);
	box_normal->image(pic_ennormal);
	box_normal->deimage(pic_disnormal);
	box_normal->redraw();
	add(box_normal);

	box_clicked = new Fl_Box(x, y, w, h);
	box_clicked->image(pic_enclicked);
	box_clicked->deimage(pic_disclicked);
	box_clicked->redraw();
	add(box_clicked);

	end();

	resizable(NULL);
	size(w, h);

	if (is_on)
	{
		is_checked = true;
		Fl_Wizard::value(box_clicked);
	}
	else
		Fl_Wizard::value(box_normal);

	redraw();
}

YabCheckboxImage::~YabCheckboxImage()
{
}

int YabCheckboxImage::value()
{
	return (int)is_checked;
}

void YabCheckboxImage::value(int is_on)
{
	if (is_on)
	{
		is_checked = true;
		Fl_Wizard::value(box_clicked);
	}
	else
	{
		is_checked = false;
		Fl_Wizard::value(box_normal);
	}
	redraw();
}

int YabCheckboxImage::handle(int event)
{
	if(active())
	{
		if (Fl::event_buttons() && Fl::event_inside(this))
		{
			is_clicked = true;
			if (is_checked)
				Fl_Wizard::value(box_normal);
			else
				Fl_Wizard::value(box_clicked);
	
			redraw();
			return 1;
		}
		else if (!Fl::event_buttons() || !Fl::event_inside(this))
		{
			if (is_clicked)
			{
				if (Fl::event_inside(this))
				{
					if (is_checked)
					{
						is_checked = false;
					}
					else
					{
						is_checked = true;
					}
					do_callback();
				}
	
				is_clicked = false;
				if (is_checked)
					Fl_Wizard::value(box_clicked);
				else
					Fl_Wizard::value(box_normal);
	
				redraw();
	
			}
		}
	}
	return Fl_Wizard::handle(event);
}
