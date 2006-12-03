#ifndef YABBOXVIEW_H
#define YABBOXVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Group.H>
#include "YabView.h"
#include "YabWidget.h"
#include "global.h"

class YabBoxView: public Fl_Group
{
public:
	YabBoxView(int x_, int y_, int w_, int h_, const char* id_, int type, const char* label_)
		:Fl_Group(x_, y_, w_, h_)
	{
		int lh = 12;
		x = x_+5; y = y_+lh;
		w = w_-10; h = h_-17;
		label = label_;
		id = id_;

		color(fl_rgb_color(B_GREY));

		Fl_Boxtype BOXTYPE = FL_NO_BOX;
		if (type == 1) BOXTYPE = FL_BORDER_FRAME;
		if (type == 2) BOXTYPE = FL_EMBOSSED_FRAME;

		mainbox = new Fl_Box(x_+2, y_+6, w_-4, h_-8);
		mainbox->box(BOXTYPE);
		mainbox->selection_color(fl_rgb_color(10, 10, 10));
		add(mainbox);

		if (strcmp(label, ""))
		{
			int lw = 10+(int)fl_width(label);
			labelbox = new Fl_Box(x+5, y_, lw, lh);
			labelbox->box(FL_FLAT_BOX);
			labelbox->color(fl_rgb_color(B_GREY));
			labelbox->labelsize(12);
			labelbox->labeltype(FL_ENGRAVED_LABEL);
			labelbox->copy_label(label);
			add(labelbox);
		}
	}
	~YabBoxView()
	{
	}

	YabView *AddView()
	{
		YabView *view = new YabView(x, y, w, h, id);
		view->end();
		add(view);
		redraw();
		return view;
	}

	void draw()
	{
		Fl_Group::draw();
	}

private:
	int x, y, w, h;
	const char *label, *id;
	Fl_Box *mainbox, *labelbox;
};
#endif
