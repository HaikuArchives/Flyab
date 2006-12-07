#include "YabPopupMenu.h"
#include "YabInterface.h"
#include "global.h"

YabPopupMenu::YabPopupMenu(int x, int y, const char* contents)
	: Fl_Double_Window(x, y, 20, 20)
{
	line_w = 0;
	line_h = 14;
	w=0; h=0;
	running = true;
	textcolor_low	= fl_rgb_color(0, 0, 0);
	textcolor_high	= fl_rgb_color(255, 255, 255);
	bgcolor_low		= fl_rgb_color(B_GREY);
	bgcolor_high	= FL_SELECTION_COLOR;
	clicked_item	= -1;
	selected_item	= -1;

	list = new Fl_Pack(0, 0, 20, 20);
	list->spacing(0);

	int n=0;
	string line="";
	for (int i=0; i<=strlen(contents); i++)
	{
		if (contents[i] == 124 || contents[i] == 0)
		{
			if (line == "--")
			{
				Fl_Box *b = new Fl_Box(0, h, 20, 2);
				b->box(FL_THIN_DOWN_BOX);
				h += 2;
			}
			else
			{
				Fl_Box *b = new Fl_Box(0, h, 20, line_h);
				b->box(FL_NO_BOX);
				b->color(bgcolor_high);
				b->labelsize(B_FONT_SIZE);
				b->labelcolor(textcolor_low);
				b->align(FL_ALIGN_CENTER);
				b->copy_label(line.c_str());
				itembox.push_back(b);
				items.push_back(line);

				if (fl_width(line.c_str()) > line_w) line_w = (int)fl_width(line.c_str());
				h += line_h;
			}
			line = "";
		}
		else
			line += contents[i];
	}
	w = line_w + 10;

	list->end();
	end();
	clear_border();
	box(FL_UP_BOX);
	color(bgcolor_low);
	resizable(list);
	size(w, h);
	redraw();
}

YabPopupMenu::~YabPopupMenu()
{
}

const char* YabPopupMenu::text(int n)
{
	if (n < 0 || n > items.size()) return "";
	return items[n].c_str();
}

int YabPopupMenu::value()
{
	return selected_item;
}

int YabPopupMenu::Run()
{
	YabInterface *yi;
	while (running)
	{
		if (!shown()) break;
		yi->Snooze(1);
	}
	return selected_item;
}

void YabPopupMenu::draw()
{
	for (int n=0; n<itembox.size(); n++)
	{
		if (Fl::event_inside(itembox[n]))
		{
			itembox[n]->labelcolor(textcolor_high);
			if (Fl::event_buttons())
				itembox[n]->box(FL_THIN_DOWN_BOX);
			else
				itembox[n]->box(FL_FLAT_BOX);
		}
		else
		{
			itembox[n]->box(FL_NO_BOX);
			itembox[n]->labelcolor(textcolor_low);
		}
		itembox[n]->redraw();
	}
	Fl_Window::draw();
}

int YabPopupMenu::handle(int event)
{
	switch (event)
	{
	case FL_DRAG:
		clicked_item = -1;
		for (int n=0; n<itembox.size(); n++)
		{
			if (Fl::event_inside(itembox[n]))
			{
				clicked_item = n;
				break;
			}
		}
	case FL_MOVE:
	case FL_LEAVE:
		selected_item = -1;
		redraw();
		break;
	case FL_UNFOCUS:
		selected_item = -1;
		running = false;
		break;
	case FL_PUSH:
		for (int n=0; n<itembox.size(); n++)
		{
			if (Fl::event_inside(itembox[n]))
			{
				clicked_item = n;
				redraw();
				break;
			}
		}
		break;
	case FL_RELEASE:
		for (int n=0; n<itembox.size(); n++)
		{
			if (Fl::event_inside(itembox[n]))
			{
				if (clicked_item == n)
				{
					selected_item = n;
					running = false;
					break;
				}
			}
		}
		break;
	};
	return Fl_Window::handle(event);
}
