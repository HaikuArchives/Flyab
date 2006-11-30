#include "YabWindow.h"
#include <FL/fl_draw.H>
	
YabWindow::YabWindow(int x, int y, int width, int height, const char* id, const char* label)
	: Fl_Double_Window(x,y,width,height)
{
	resizable(this);
	copy_label(label);
	id_ = id;
	minw = minh = 0;
	maxw = maxh = -1;
}

void YabWindow::MinimumTo(int w, int h)
{
	minw = w;
	minh = h;
	size_range(minw, minh, maxw, maxh);
}

void YabWindow::MaximumTo(int w, int h)
{
	maxw = w;
	maxh = h;
	size_range(minw, minh, maxw, maxh);
}

int YabWindow::GetMinimumWidth()
{
	return minw;
}

int YabWindow::GetMinimumHeight()
{
	return minh;
}

int YabWindow::GetMaximumWidth()
{
	return maxw;
}

int YabWindow::GetMaximumHeight()
{
	return maxh;
}

const char* YabWindow::GetID()
{
	return id_.c_str();
}

int YabWindow::handle(int handle_)
{
	return Fl_Window::handle(handle_);
}

