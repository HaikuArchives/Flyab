#ifndef YABCHECKBOXIMAGE_H
#define YABCHECKBOXIMAGE_H

#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Wizard.H>
#include <YabWidget.h>

class YabCheckboxImage: public Fl_Wizard, public YabWidget
{
public:
	YabCheckboxImage(int x, int y, const char* id, const char* en_on, const char* en_off, const char* dis_on, const char* dis_off, int is_on);
	~YabCheckboxImage();
	int value();
	void value(int is_on);
	int handle(int event);

private:
	bool is_clicked, is_checked;
	Fl_Box *box_normal, *box_clicked;
	Fl_PNG_Image *pic_ennormal, *pic_enclicked, *pic_disnormal, *pic_disclicked;

};
#endif
