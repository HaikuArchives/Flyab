#include <FL/Fl.H>
#include <iostream>
#include "YabButtonImage.h"

YabButtonImage::YabButtonImage(int x, int y, const char* id, const char* clicked, const char* normal, const char* disabled)
	:Fl_Wizard(x, y, 10, 10), YabWidget(id)
{
	box(FL_NO_BOX);
	is_clicked = false;
	pic_normal = new Fl_PNG_Image(normal);
	pic_clicked = new Fl_PNG_Image(clicked);
	pic_disabled = new Fl_PNG_Image(disabled);

	int w = pic_normal->w();
	int h = pic_normal->h();

	box_normal = new Fl_Box(x, y, w, h);
	box_normal->image(pic_normal);
	box_normal->deimage(pic_disabled);
	box_normal->redraw();
	add(box_normal);

	box_clicked = new Fl_Box(x, y, w, h);
	box_clicked->image(pic_clicked);
	box_clicked->deimage(pic_disabled);
	box_clicked->redraw();
	add(box_clicked);

	end();

	resizable(NULL);
	size(w, h);
	value(box_normal);
	redraw();
}

YabButtonImage::~YabButtonImage()
{
}

int YabButtonImage::handle(int event)
{
	if(active())
	{
		if (Fl::event_buttons() && Fl::event_inside(this))
		{
			if (!is_clicked)
			{
				is_clicked = true;
				value(box_clicked);
				redraw();
				return 1;
			}
		}
		else if (!Fl::event_buttons() || !Fl::event_inside(this))
		{
			if (is_clicked)
			{
				is_clicked = false;
				value(box_normal);
				redraw();
				if (Fl::event_inside(this)) do_callback();
			}
		}
	}
	return Fl_Wizard::handle(event);
}
