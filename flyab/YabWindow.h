#ifndef YABWINDOW_H
#define YABWINDOW_H

#include <FL/Fl_Double_Window.H>
#include <string>

class YabWindow : public Fl_Double_Window
{
public:
	YabWindow(int x1, int y1, int width, int height, const char* id, const char* label);
	const char* GetID();
	int handle(int);

private:
	std::string id_;
};

#endif
