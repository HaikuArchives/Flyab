#include <FL/Fl_Choice.H>
#include "YabWidget.h"
#include "global.h"

class YabDropBox: public Fl_Choice, public YabWidget
{
public:
	YabDropBox(int x1, int y1, int width, int height, const char* id, const char* label)
		: Fl_Choice(x1,y1,width,height), YabWidget(id)
	{
		copy_label(label);
		textsize(B_FONT_SIZE);
		labelsize(B_FONT_SIZE);
		color(fl_rgb_color(B_GREY));
		int w = 5+(int)fl_width(label);
		resize(x1+w, y1, width-w, height);
	}
};
