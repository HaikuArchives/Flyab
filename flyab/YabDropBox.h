#include <FL/Fl_Choice.H>
#include <FL/fl_draw.H>
#include "YabWidget.h"
#include "global.h"

class YabDropBox: public Fl_Choice, public YabWidget
{
public:
	YabDropBox(int x1, int y1, int width, int height, const char* id, const char* label)
		: Fl_Choice(x1,y1,width,height), YabWidget(id)
	{
		textsize(B_FONT_SIZE);
		labelsize(B_FONT_SIZE);
		copy_label(label);
		color(fl_rgb_color(B_GREY));
		int w = 5 + static_cast<int>(fl_width(label));
		resize(x1+w, y1, width-w, height);
	}
};
