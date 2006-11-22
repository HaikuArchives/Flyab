#include "YabWindow.h"
	
YabWindow::YabWindow(int x, int y, int width, int height, const char* id, const char* label)
	: Fl_Double_Window(x,y,width,height)
{
	resizable(this);
	copy_label(label);
	id_ = id;
}

const char* YabWindow::GetID()
{
	return id_.c_str();
}

int YabWindow::handle(int handle_)
{
	return Fl_Window::handle(handle_);
}

