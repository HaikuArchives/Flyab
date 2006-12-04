#ifndef YABBUTTONIMAGE_H
#define YABBUTTONIMAGE_H

#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Wizard.H>
#include <YabWidget.h>

class YabButtonImage: public Fl_Wizard, public YabWidget
{
public:
	YabButtonImage(int x, int y, const char* id, const char* clicked, const char* normal, const char* disabled)
		:Fl_Wizard(x, y, 60, 20), YabWidget(id)
	{
		box(FL_NO_BOX);
		is_clicked = false;
		pic_normal = new Fl_PNG_Image(normal);
		pic_clicked = new Fl_PNG_Image(clicked);
		pic_disabled = new Fl_PNG_Image(disabled);
		end();

		int w = pic_normal->w();
		int h = pic_normal->h();

		box_normal = new Fl_Box(x, y, w, h);
		box_normal->image(pic_normal);
		box_normal->redraw();

		box_clicked = new Fl_Box(x, y, w, h);
		box_clicked->image(pic_clicked);
		box_clicked->redraw();

		box_disabled = new Fl_Box(x, y, w, h);
		box_disabled->image(pic_disabled);
		box_disabled->redraw();

		add(box_normal);
		add(box_clicked);
		add(box_disabled);
		end();

		size(w, h);
		resizable(NULL);
		value(box_normal);
		redraw();
	}

	~YabButtonImage()
	{
	}

	int handle(int event)
	{
		if (event == FL_PUSH && Fl::event_inside(this))
		{
			is_clicked = true;
			value(box_clicked);
			redraw();
			do_callback();
			return 1;
		}
		else if (event == FL_RELEASE && is_clicked)
		{
			is_clicked = false;
			value(box_normal);
			redraw();
		}
		return Fl_Wizard::handle(event);
	}

private:
	bool is_clicked;
	Fl_Box *box_normal, *box_clicked, *box_disabled;
	Fl_PNG_Image *pic_normal, *pic_clicked, *pic_disabled;

};
#endif
