#ifndef YABBUTTONIMAGE_H
#define YABBUTTONIMAGE_H

#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Wizard.H>
#include <YabWidget.h>

class YabButtonImage: public Fl_Wizard, public YabWidget
{
public:
	YabButtonImage(int x, int y, const char* id, const char* clicked, const char* normal, const char* disabled)
		:Fl_Wizard(x, y, 20, 20), YabWidget(id)
	{
		is_clicked = false;
		pic_normal = new Fl_PNG_Image(normal);
		pic_clicked = new Fl_PNG_Image(clicked);
		pic_disabled = new Fl_PNG_Image(disabled);

		int w = pic_normal->w();
		int h = pic_normal->h();
printf("w = %d  -  h = %d\n", w, h);
		group_normal = new Fl_Group(x, y, w, h);
		group_normal->image(pic_normal);
		group_normal->end();

		group_clicked = new Fl_Group(x, y, w, h);
		group_clicked->image(pic_clicked);
		group_clicked->end();

		group_disabled = new Fl_Group(x, y, w, h);
		group_disabled->image(pic_disabled);
		group_disabled->end();

		add(group_normal);
		add(group_clicked);
		add(group_disabled);
		end();
		resizable(group_normal);
		resizable(group_clicked);
		resizable(group_disabled);
		size(w, h);
		resizable(NULL);
		value(group_normal);
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
			value(group_clicked);
			redraw();
			do_callback();
			return 1;
		}
		else if (event == FL_RELEASE && is_clicked)
		{
			is_clicked = false;
			value(group_normal);
			redraw();
		}
		return Fl_Wizard::handle(event);
	}

private:
	bool is_clicked;
	Fl_Group *group_normal, *group_clicked, *group_disabled;
	Fl_PNG_Image *pic_normal, *pic_clicked, *pic_disabled;

};
#endif
