#ifndef YABBUTTONIMAGE_H
#define YABBUTTONIMAGE_H

#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Wizard.H>
#include <YabWidget.h>

class YabButtonImage: public Fl_Wizard, public YabWidget
{
public:
	YabButtonImage(int x, int y, const char* id, const char* clicked, const char* normal, const char* disabled);
	~YabButtonImage();
	int handle(int event);

private:
	bool is_clicked;
	Fl_Box *box_normal, *box_clicked;
	Fl_PNG_Image *pic_normal, *pic_clicked, *pic_disabled;

};
#endif
