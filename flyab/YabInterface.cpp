#include "BRect.h"
#include "BPoint.h"

#define _L(str) str

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_File_Icon.H>
#include <FL/Fl_Value_Slider.H>

#include "global.h"
#include "YabInterface.h"
#include "YabAlert.h"
#include "YabBoxView.h"
#include "YabButton.h"
#include "YabCheckButton.h"
#include "YabColorControl.h"
#include "YabDropBox.h"
#include "YabListBox.h"
#include "YabMenuBar.h"
#include "YabProgressBar.h"
#include "YabRadioButton.h"
#include "YabSlider.h"
#include "YabSpinControl.h"
#include "YabStackView.h"
#include "YabTabView.h"
#include "YabTextControl.h"
#include "YabView.h"
#include "YabWindow.h"

static bool localize = false;
static bool quitting = false;
static int lastkey = 0;
static std::string localMessage = "";
static std::string currentLib;

//static property_info prop_list[] = { 
//	{ "YabSendString", {B_SET_PROPERTY, 0}, {B_NAME_SPECIFIER, 0}, "Send a string to MESSAGE$"},
//	0 // terminate list 
//}; 

/**
 * Start the interpreter thread
 */

struct thread_data{
		int  argc;
		char** argv;
		YabInterface *yab;
};

static thread_data t;

void *interpreter(void *data)
{
	int argc,t;
	char **argv;
	YabInterface *yab;

	thread_data *myData = (thread_data*)data;
	argc = myData->argc;
	argv = myData->argv;
	yab = myData->yab;

	t = mmain(argc,argv, yab);
	quitting = true;
	return (void*)t;
}

/**
 * Constructor sets application directory, spawn the interpreter thread
 */
YabInterface::YabInterface(int argc, char **argv, const char* signature)
{
	// we want system icons
	Fl_File_Icon::load_system_icons();

	Fl::lock();
	t.argc = argc;
	t.argv = argv;
	t.yab = this;

	// !! LINUX ONLY STUFF
	int i = pthread_create(&myThread, NULL, interpreter, (void*)&t);

	// strcpy(ApplicationDirectory, AppDirectory.Path());

	localMessage = "";

	// viewList = new YabList();
	// yabbitmaps = new BList();
	// yabcanvas = new BList();
	drawStroking = false;
	// yabPattern = B_SOLID_HIGH;
	yabAlpha = 255;
	errorCode = 0;
	// Roster = NULL;
	currentLineNumber = -1;
	exiting = false;
	
	for(int i=0; i<63; i++)
		mousemessagebuffer[i] = ' ';
	mousemessagebuffer[63] = '\0';

	Fl::scheme(YAB_SCHEME); 
	Fl::visual(FL_DOUBLE|FL_INDEX|FL_RGB);
	fl_font(0, B_FONT_SIZE);

	// myProps = new BPropertyInfo(prop_list);
	currentLib = "";
}

YabInterface::~YabInterface()
{
	// delete mainFileName;
	// delete song;
	// delete fopen;
	// delete fsave;
	// delete viewList;
	// delete Roster;
	// delete myProps;
	// while(yabbitmaps->CountItems()>0)
	// {
	// 	int i = 0;
	// 	BBitmap *b = (BBitmap*)yabbitmaps->RemoveItem(i);
	// 	delete b;
	// }
}

void YabInterface::Run()
{
	while(!quitting)
	{
		Fl::wait(0.1);
		lastkey = Fl::event_key();
	}
	while(Fl::first_window())
		Fl::first_window()->hide();
}

/**
 * Returns the application directory
 */
const char* YabInterface::GetApplicationDirectory()
{
	return (const char*) ApplicationDirectory;
}

/*status_t YabInterface::GetSupportedSuites(BMessage *msg) 
{ 
	msg->AddString("suites", "suite/vnd.yab-YabInterface"); 
	BPropertyInfo prop_info(prop_list); 
	msg->AddFlat("messages", &prop_info); 
	return BApplication::GetSupportedSuites(msg); 
} 

BHandler* YabInterface::ResolveSpecifier(BMessage *msg, int index, BMessage *spec, int form, const char *prop) 
{ 
	if (myProps->FindMatch(msg, index, spec, form, prop) >= 0) 
		return (BHandler*)this; 
	return BApplication::ResolveSpecifier(msg, index, spec, form, prop); 
}

void YabInterface::MessageReceived(BMessage *message)
{
	// message->PrintToStream();
	switch(message->what)
	{
		case B_SET_PROPERTY:
			{
				BMessage msg;
				int i,w;
				const char *prop;

				if(message->GetCurrentSpecifier(&i, &msg, &w, &prop) != B_BAD_SCRIPT_SYNTAX)
				{
					BString s;
					msg.FindString("name", &s);
					localMessage += "_Scripting:";
					localMessage += s;
					localMessage +="|";
				}
			}
			break;
		case B_REFS_RECEIVED:
			{
				entry_ref ref;
				if(message->FindRef("refs", 0, &ref)==B_OK)
				{
					BEntry e(&ref);
					BPath path;
					e.GetPath(&path);
					localMessage += "_RefsReceived:";
					localMessage += path.Path();
					localMessage +="|";
				}
			}
			break;
		default:
			BApplication::MessageReceived(message);
			break;
	}
} */

/**
 * The QUIT_REQUESTED message arrived. If the interpreter thread is still active,
 * kill it, otherwise exit directly.
 */
bool YabInterface::QuitRequested()
{
	exiting = true;
	return false;
}

/*
void YabInterface::RefsReceived(BMessage *message){
	entry_ref ref;
	BString tempstr;
	if(message->FindRef("refs", 0, &ref)==B_OK)
	{
		BEntry e(&ref);
		BPath path;
		e.GetPath(&path);
		tempstr += path.Path();
		refsRec = strdup(tempstr.String());
	}
} */

bool YabInterface::ExitRequested()
{
	// Quit();
	return true;
}

//void YabInterface::MessageCallback(Fl_Widget *widget, void *data)
void YabInterface::StaticMessageCallback(Fl_Widget *widget, void *data=0)
{
	std::string t = "";
	if(YabWindow *win = dynamic_cast<YabWindow*>(widget))
	{
		t += win->GetID();
		t += ":_QuitRequested|";
		localMessage += t;
		return;
	}
	if(YabMenuBar *menu = dynamic_cast<YabMenuBar*>(widget))
	{
		t += ((YabView*)menu->parent())->GetID();
		t += ":";
		t += (const char*)data;
		t += "|";
		localMessage += t;
		return;
	}
	if(YabCheckButton *check = dynamic_cast<YabCheckButton*>(widget))
	{
		t += check->GetID();
		if(check->value() == 0)
			t += ":OFF|";
		else
			t += ":ON|";
		localMessage += t;
		return;
	}
	if(YabRadioButton *radio = dynamic_cast<YabRadioButton*>(widget))
	{
		t += radio->GetID();
		t += "|";
		localMessage += t;
		return;
	}
	if(YabButton *button = dynamic_cast<YabButton*>(widget))
	{
		t += button->GetID();
		t += "|";
		localMessage += t;
		return;
	}
	if(YabTextControl *txt = dynamic_cast<YabTextControl*>(widget))
	{
		t += txt->GetID();
		t += ":";
		t += txt->value();
		t += "|";
		localMessage += t;
		return;
	}
	if(YabDropBox *db = dynamic_cast<YabDropBox*>(widget))
	{
		int n = db->value();
		t += db->GetID();
		t += ":";
		t += db->text(n);
		t += "|";
		localMessage += t;
		return;
	}
	if(YabListBox *list = dynamic_cast<YabListBox*>(widget))
	{
		std::stringstream s;
		if(list->value() && s << list->value())
		{
			t += list->GetID();
			if(Fl::event_key() == FL_Enter || Fl::event_clicks()>0)
			{
				t += ":_Invoke:";
			}
			else
				t += ":_Select:";
			Fl::event_clicks(0);
			t += s.str();
			t += "|";
		}
		localMessage += t;
		return;
	}
	if(YabSpinControl *spin = dynamic_cast<YabSpinControl*>(widget))
	{
		std::stringstream s;
		s << spin->GetID();
		s << ":";
		s << spin->value();
		s << "|";
		localMessage += s.str();
		return;
	}
	if(YabSlider *yabslider = dynamic_cast<YabSlider*>(widget))
	{
		std::stringstream s;
		s << yabslider->GetID();
		s << ":";
		s << yabslider->value();
		s << "|";
		localMessage += s.str();
		return;
	}
}
/*
void YabInterface::StaticMessageCallback(Fl_Widget *widget, void *yab)
{
	((YabInterface*)yab)->MessageCallback(widget);
}
*/
BPoint YabInterface::GetWindowCoordinates(Fl_Widget *parent, double x, double y)
{
	BPoint ret(x,y);
	while(dynamic_cast<Fl_Window*>(parent) == NULL)
	{
		ret.x += (double)parent->x();
		ret.y += (double)parent->y();
		parent = parent->parent();
	}
	return ret;
}

/**
 * Open a window, add the main view.
 */
void YabInterface::OpenWindow(const BRect frame, const char* id, const char* title)
{
	Fl::lock();
	YabWindow *window = new YabWindow((int)frame.x1,(int)frame.y1, (int)frame.width, (int)frame.height, id, title);
	window->copy_label(title);
	window->color(fl_rgb_color(B_GREY));
	window->resizable(window);
	window->callback(StaticMessageCallback);

	YabView *winView = new YabView(0,0, (int)frame.width, (int)frame.height, id);
	winView->callback(StaticMessageCallback);
	yabViewList.push_back(winView);

	window->show();
	Fl::unlock();
	// Fl::awake();

	// YabWindow* w = new YabWindow(frame,title,id, B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_ASYNCHRONOUS_CONTROLS);
	// YabView* myView = new YabView(w->Bounds(), id, B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_NAVIGABLE_JUMP);
	// w->Lock();
	// 	w->AddChild(myView);

	// 	viewList->AddView(id, myView, TYPE_YABVIEW);
		// viewList->PrintOut();

		// w->Minimize();
	// 	w->SetSizeLimits(10,3000,10,3000);
	// 	w->Show();
	// w->Unlock();
	// w->layout = -1;
}

int YabInterface::CloseWindow(const char* view)
{
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			Fl::lock();
			Fl_Window *w = yabViewList[i]->window();
			w->hide();
			Fl::unlock();
			return 1;
		}
	}
}

void YabInterface::WindowSet(const char* option, const char* value, const char* window)
{
	int myMode=0;
	std::string t = option;
	std::transform(t.begin(),t.end(),t.begin(),(int (*)(int))std::tolower);

	if(t.find("look") != std::string::npos)
	{
		if(t.find("documented") != std::string::npos) myMode = 101;
		if(t.find("titled") != std::string::npos) myMode = 102;
		if(t.find("floating") != std::string::npos) myMode = 103;
		if(t.find("modal") != std::string::npos) myMode = 104;
		if(t.find("bordered") != std::string::npos) myMode = 105;
		if(t.find("no-border") != std::string::npos) myMode = 106;
	}

	if(t.find("feel") != std::string::npos)
	{
		if(t.find("normal") != std::string::npos) myMode = 201;
		if(t.find("modal-app") != std::string::npos) myMode = 202;
		if(t.find("modal-all") != std::string::npos) myMode = 203;
		if(t.find("floating-app") != std::string::npos) myMode = 204;
		if(t.find("floating-all") != std::string::npos) myMode = 205;
	}

	if(t.find("title") != std::string::npos) myMode = 301;

	if(t.find("flags") != std::string::npos)
	{
		if(t.find("not-closable") != std::string::npos) myMode = 401;
		if(t.find("not-zoomable") != std::string::npos) myMode = 402;
		if(t.find("not-minimizable") != std::string::npos) myMode = 403;
		if(t.find("not-h-resizable") != std::string::npos) myMode = 404;
		if(t.find("not-v-resizable") != std::string::npos) myMode = 405;
		if(t.find("not-resizable") != std::string::npos) myMode = 406;
		if(t.find("not-workspace-activation") != std::string::npos) myMode = 407;
		if(t.find("accept-first-click") != std::string::npos) myMode = 408;
		if(t.find("reset") != std::string::npos) myMode = 409;
	}

	if(t.find("workspace") != std::string::npos)
	{
		if(t.find("all") != std::string::npos) myMode = 501;
		if(t.find("current") != std::string::npos) myMode = 502;
	}

	if (myMode == 0) ErrorGen("Invalid option");

	std::string s = window;
	YabWindow *win;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if (win = dynamic_cast<YabWindow*>(yabViewList[i]->window()));
		{
			if(s == win->GetID())
			{
				Fl::lock();
				switch (myMode)
				{
				// look
					case 101:
						win->border(1);
						break;
					case 102:
						win->border(1);
						break;
					case 103:
						win->border(1);
						break;
					case 104:
						win->border(1);
						break;
					case 105:
						win->border(1);
						break;
					case 106:	// no-border
						win->border(0);
						break;
				// feel
					case 201:
						break;
					case 202:	// modal-app
						win->set_modal();
						break;
					case 203:	// modal-all
						win->set_modal();
						break;
					case 204:
						break;
					case 205:
						break;
				// title
					case 301:	// title
						win->label(value);
						break;
				// flags
					case 401:
						break;
					case 402:
						break;
					case 403:
						break;
					case 404:	// not-h-resizable
						{
							int w = win->w();
							int minh = win->GetMinimumHeight();
							int maxh = win->GetMaximumHeight();
							win->MinimumTo(w, minh);
							win->MaximumTo(w, maxh);
						}
						break;
					case 405:	// not-v-resizable
						{
							int h = win->h();
							int minw = win->GetMinimumWidth();
							int maxw = win->GetMaximumWidth();
							win->MinimumTo(minw, h);
							win->MaximumTo(maxw, h);
						}
						break;
					case 406:	// not-resizable
						win->resizable(NULL);
						break;
					case 407:
						break;
					case 408:
						break;
					case 409:
						break;
				// workspace
					case 501:
						break;
					case 502:
						break;
				};
				Fl::unlock();
				return;
			}
		}
	}
	Error(window, "WINDOW");
}

void YabInterface::WindowSet(const char* option, const char* window)
{
	int myMode=0;
	std::string t = option;
	std::transform(t.begin(),t.end(),t.begin(),(int (*)(int))std::tolower);

	if(t.find("activate") != std::string::npos) myMode = 1;
	if(t.find("deactivate") != std::string::npos) myMode = 2;
	if(t.find("minimize") != std::string::npos) myMode = 3;
	if(t.find("maximize") != std::string::npos) myMode = 4;
	if(t.find("enable-updates") != std::string::npos) myMode = 5;
	if(t.find("disable-updates") != std::string::npos) myMode = 6;
	if (myMode == 0) ErrorGen("Invalid option");

	std::string v = window;
	YabWindow *win;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if (win = dynamic_cast<YabWindow*>(yabViewList[i]->window()));
		{
			if(v == win->GetID())
			{
				Fl::lock();
				switch (myMode)
				{
				case 1:	// Activate the window, so it is in the foreground.
					win->activate();
					break;
				case 2:	// Deactivate the window, so it is in the background.
					// fltk docu says, deactivate() currently doesn't work for windows
					win->deactivate();
					break;
				case 3:	// Minimize the window, or restore the window if it is already minimized.
					break;
				case 4:	// Maximize (zoom) the window.
					break;
				case 5:	// Updates the window again after a "Disable-Updates".
					break;
				case 6:	// Disables the automatic window updates.
					break;
				};
				Fl::unlock();
				return;
			}
		}
	}
	Error(window, "WINDOW");
}

void YabInterface::WindowSet(const char* option, int r, int g, int b, const char* window)
{
	int opt = 0;
	std::string t = option;
	std::transform(t.begin(),t.end(),t.begin(),(int (*)(int))std::tolower);
	if(t.find("highcolor",0) != std::string::npos)
		opt = 1;
	else if(t.find("lowcolor",0) != std::string::npos)
		opt = 2;
	else if(t.find("bgcolor",0) != std::string::npos)
		opt = 3;
	else ErrorGen("Invalid option");

	std::string v = window;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(v == yabViewList[i]->GetID())
		{
			Fl::lock();
			switch(opt)
			{
				case 1: {
						YabDrawing *t = new YabDrawing();
						t->command = 6;
						t->r = r;
						t->g = g;
						t->b = b;
						yabViewList[i]->AddDrawing(t);
					}
					break;
				case 2: {
						YabDrawing *t = new YabDrawing();
						t->command = 7;
						t->r = r;
						t->g = g;
						t->b = b;
						yabViewList[i]->AddDrawing(t);
					}
					break;
				case 3: ((Fl_Widget*)yabViewList[i])->color(fl_rgb_color(r,g,b)); //not working ?!?
					yabViewList[i]->redraw();
					break;
			}
			Fl::unlock();
			return;
		}
	}
	Error(window, "VIEW or WINDOW");
}

void YabInterface::WindowSet(const char* option, double x, double y, const char* window)
{
	int myMode=0;

	std::string t = option;
	std::transform(t.begin(),t.end(),t.begin(),(int (*)(int))std::tolower);

	if(t.find("resizeto") != std::string::npos) myMode = 1;
	if(t.find("moveto") != std::string::npos) myMode = 2;
	if(t.find("minimumto") != std::string::npos) myMode = 3;
	if(t.find("maximumto") != std::string::npos) myMode = 4;
	if (myMode == 0) Error(option, "OPTION");

	int w = static_cast<int>(x);
	int h = static_cast<int>(y);
	std::string s = window;

	for (int i = 0; i < yabViewList.size(); i++)
		if (s == yabViewList[i]->GetID())
		{
			Fl::lock();
			YabWindow *win = dynamic_cast<YabWindow*>(yabViewList[i]->window());
			if (myMode == 1) win->size(w, h);
			if (myMode == 2) win->position(w, h);
			if (myMode == 3) win->MinimumTo(w, h);
			if (myMode == 4) win->MaximumTo(w, h);
			Fl::unlock();
			return;
		}
	Error(window, "WINDOW");
}

void YabInterface::WindowClear(const char* window)
{
	printf("-- isn't that an old commad?");
} 

int YabInterface::WindowGet(const char* view, const char* option)
{
	std::string s = view;
	int myMode=0, ret;

	std::string t = option;
	std::transform(t.begin(),t.end(),t.begin(),(int (*)(int))std::tolower);

	if(t.find("position-x") != std::string::npos) myMode = 1;
	if(t.find("position-y") != std::string::npos) myMode = 2;
	if(t.find("width") != std::string::npos) myMode = 3;
	if(t.find("height") != std::string::npos) myMode = 4;
	if(t.find("minimum-width") != std::string::npos) myMode = 5;
	if(t.find("minimum-height") != std::string::npos) myMode = 6;
	if(t.find("maximum-width") != std::string::npos) myMode = 7;
	if(t.find("maximum-height") != std::string::npos) myMode = 8;
	if(t.find("exists") != std::string::npos) myMode = 9;
	if (myMode == 0) Error(option, "OPTION");

	for (int i = 0; i < yabViewList.size(); i++)
		if (s == yabViewList[i]->GetID())
		{
			Fl::lock();
			YabWindow *win = dynamic_cast<YabWindow*>(yabViewList[i]->window());
			if (myMode == 1) ret = win->x();
			if (myMode == 2) ret = win->y();
			if (myMode == 3) ret = win->w();
			if (myMode == 4) ret = win->h();
			if (myMode == 5) ret = win->GetMinimumHeight();
			if (myMode == 6) ret = win->GetMinimumWidth();
			if (myMode == 7) ret = win->GetMaximumHeight();
			if (myMode == 8) ret = win->GetMaximumWidth();
			if (myMode == 9) ret = 1;
			Fl::unlock();
			return ret;
		}
	if (myMode == 9)
		return 0;
	else
		Error(view, "WINDOW");
}

void YabInterface::View(BRect frame, const char* id, const char* view)
{
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			Fl::lock();

			BPoint newCoor = GetWindowCoordinates(yabViewList[i], frame.x1, frame.y1);

			YabView *newView = new YabView((int)newCoor.x,(int)newCoor.y, (int)frame.width, (int)frame.height, id);
			newView->end();
			newView->color(fl_rgb_color(B_GREY));
			newView->callback(StaticMessageCallback);
			yabViewList[i]->add(newView);
			yabViewList[i]->window()->show();
			yabViewList.push_back(newView);
			
			Fl::unlock();

			return;
		}
	}

	Error(view, "VIEW");
}

int YabInterface::ViewGet(const char* view, const char* option) 
{
}

void YabInterface::BoxView(BRect frame, const char* id, const char* text, int lineType, const char* view)
{
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			Fl::lock();

			BPoint newCoor = GetWindowCoordinates(yabViewList[i], frame.x1, frame.y1);
			YabBoxView *box = new YabBoxView((int)newCoor.x, (int)newCoor.y, (int)frame.width, (int)frame.height, id, lineType, text);

			YabView *view = box->AddView();
			view->color(fl_rgb_color(B_GREY));
			view->callback(StaticMessageCallback);

			yabViewList.push_back(view);
			yabViewList[i]->add(box);
			yabViewList[i]->redraw();

			Fl::unlock();
			return;
		}
	}
	Error(view, "VIEW");
}

void YabInterface::Tab(BRect frame, const char* id, const char* mode, const char* view)
{
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			Fl::lock();

			BPoint newCoor = GetWindowCoordinates(yabViewList[i], frame.x1, frame.y1);
			YabTabView *tabs = new YabTabView((int)newCoor.x, (int)newCoor.y, (int)frame.width, (int)frame.height, id);

			yabViewList[i]->add(tabs);
			yabViewList[i]->redraw();
			Fl::unlock();
			return;
		}
	}
	Error(view, "VIEW");
}

void YabInterface::TabAdd(const char* id, const char* tabname)
{
	std::string b=id;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if (YabTabView *tabs = dynamic_cast<YabTabView*>(yabViewList[i]->child(j)))
			{
				if (b == tabs->GetID())
				{
					Fl::lock();

					YabView *view = tabs->NewTab(tabname);
					view->color(fl_rgb_color(B_GREY));
					view->callback(StaticMessageCallback);

					yabViewList[i]->redraw();
					yabViewList.push_back(view);

					Fl::unlock();
					return;
				}
			}
		}
	}
	Error(id, "TABVIEW");
}

void YabInterface::TabSet(const char* id, int num)
{
	std::string b=id;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if (YabTabView *tabs = dynamic_cast<YabTabView*>(yabViewList[i]->child(j)))
			{
				if (b == tabs->GetID())
				{
					Fl::lock();
					tabs->Set(num);
					Fl::unlock();
					return;
				}
			}
		}
	}
	Error(id, "TABVIEW");
}

int YabInterface::TabViewGet(const char* id)
{
	std::string b=id;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if (YabTabView *tabs = dynamic_cast<YabTabView*>(yabViewList[i]->child(j)))
			{
				if (b == tabs->GetID())
				{
					Fl::lock();
					int num = tabs->Get();
					Fl::unlock();
					return num;
				}
			}
		}
	}
	Error(id, "TABVIEW");
}

void YabInterface::TabDel(const char* id, int num)
{
}

void YabInterface::StackViews(BRect frame, const char* id, int number, const char* view)
{
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			Fl::lock();

			BPoint newCoor = GetWindowCoordinates(yabViewList[i], frame.x1, frame.y1);
			YabStackView *stack = new YabStackView((int)newCoor.x, (int)newCoor.y, (int)frame.width, (int)frame.height, id);

			for (int j=0; j<number; j++)
			{
				YabView *view = stack->NewView();
				view->color(fl_rgb_color(B_GREY));
				view->callback(StaticMessageCallback);
				yabViewList.push_back(view);
			}
			stack->end();
			yabViewList[i]->add(stack);
			yabViewList[i]->redraw();

			Fl::unlock();
			return;
		}
	}
	Error(view, "VIEW");
}

void YabInterface::StackViews(const char* id, int num)
{
	std::string b=id;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if (YabStackView *stack = dynamic_cast<YabStackView*>(yabViewList[i]->child(j)))
			{
				if (b == stack->GetID())
				{
					Fl::lock();
					stack->Set(num);
					Fl::unlock();
					return;
				}
			}
		}
	}
	Error(id, "STACKVIEW");
}

int YabInterface::StackViewGet(const char* id)
{
	std::string b=id;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if (YabStackView *stack = dynamic_cast<YabStackView*>(yabViewList[i]->child(j)))
			{
				if (b == stack->GetID())
				{
					Fl::lock();
					int num = stack->Get();
					Fl::unlock();
					return num;
				}
			}
		}
	}
	Error(id, "STACKVIEW");
}

void YabInterface::CreateButton(BRect frame, const char* id, const char* title, const char* view)
{
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			Fl::lock();

			BPoint newCoor = GetWindowCoordinates(yabViewList[i], frame.x1, frame.y1);

			YabButton *button = new YabButton((int)newCoor.x, (int)newCoor.y, (int)frame.width, (int)frame.height, id, title);
			button->callback(StaticMessageCallback);
			button->color(fl_rgb_color(B_GREY));
			button->labelsize(B_FONT_SIZE);
			yabViewList[i]->add(button);
			yabViewList[i]->redraw();

			Fl::unlock();

			return;
		}
	} 
	Error(view, "VIEW");
}

int YabInterface::CreateImage(BPoint coordinates, const char* FileName, const char* window)
{
}

int YabInterface::CreateImage(BRect frame, const char* FileName, const char* window)
{
}

int YabInterface::CreateSVG(BRect frame, const char* FileName, const char* window)
{
}

void YabInterface::StatusBar(BRect frame, const char* id, const char* label1, const char* label2, const char* view)
{
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			Fl::lock();
			BPoint newCoor = GetWindowCoordinates(yabViewList[i], frame.x1, frame.y1);

			YabProgressBar *bar = new YabProgressBar(id, (int)newCoor.x, (int)newCoor.y, (int)frame.width, (int)frame.height, label1);
			bar->labelsize(B_FONT_SIZE);
			bar->minimum(0);
			bar->maximum(100);

			yabViewList[i]->add(bar);
			yabViewList[i]->redraw();
			Fl::unlock();
			return;
		}
	}
	Error(view, "VIEW");
}

void YabInterface::StatusBarSet(const char* id, const char* label1, const char* label2, double state)
{
	std::string b=id;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if (YabProgressBar *bar = dynamic_cast<YabProgressBar*>(yabViewList[i]->child(j)))
			{
				if (b == bar->GetID())
				{
					Fl::lock();
					bar->value(state);
					bar->copy_label(label1);
					bar->redraw();
					Fl::unlock();
					return;
				}
			}
		}
	}
	Error(id, "STATUSBAR");
}

void YabInterface::CreateMenu(const char* menuhead, const char* menuitem, const char *shortcut, const char* view)
{
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			Fl::lock();
			YabMenuBar *menu;
			if (yabViewList[i]->HasMenu() == false)
			{
				BPoint newCoor = GetWindowCoordinates(yabViewList[i], 0, 0);
				menu = new YabMenuBar((int)newCoor.x, (int)newCoor.y, yabViewList[i]->w(), 20);
				yabViewList[i]->add(menu);
				yabViewList[i]->HasMenu(true);
			}
			else
			{
				for(int j = 0; j < yabViewList[i]->children(); j++)
				{
					if(menu = dynamic_cast<YabMenuBar*>(yabViewList[i]->child(j)))
						break;
				}
			}
			menu->add(menuhead, menuitem, "", shortcut, StaticMessageCallback);

			yabViewList[i]->redraw();
			Fl::unlock();
			return;
		}
	}
	Error(view, "VIEW");
}

void YabInterface::Menu(const char* menuHead, int isRadio, const char* view)
{
	if (isRadio < 1) return;
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			for(int j = 0; j < yabViewList[i]->children(); j++)
			{
				if(YabMenuBar *menu = dynamic_cast<YabMenuBar*>(yabViewList[i]->child(j)))
				{
					bool found=false;
					bool wrong_sub=false;
					bool is_sub=false;
					for (int j=0; j<menu->size();j++)
					{
						const char* item = menu->text(j);
						if (wrong_sub)
						{
							if (item == NULL) wrong_sub = false;
							continue;
						}

						if (found)
						{
							int m = menu->mode(j);
							if (m == FL_SUBMENU) is_sub = true;
							if (is_sub)
							{
								if (item == NULL) is_sub = false;
								continue;
							}
							if (item == NULL) break;
							menu->mode(j, m|FL_MENU_RADIO);
						}

						if (item && strcmp(item, menuHead) != 0 && menu->mode(j) == FL_SUBMENU)
							wrong_sub = true;
						else if (item && strcmp(item, menuHead) == 0 && menu->mode(j) == FL_SUBMENU)
							found = true;
					}
					return;
				}
			}
		}
	}
	Error(view, "VIEW");
}

void YabInterface::Menu3(const char* menuHead, const char* menuItem, const char* option, const char* view)
{
	std::string s = view;

	std::string t = option;
	std::transform(t.begin(),t.end(),t.begin(),(int (*)(int))std::tolower);

	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			for(int j = 0; j < yabViewList[i]->children(); j++)
			{
				if(YabMenuBar *menu = dynamic_cast<YabMenuBar*>(yabViewList[i]->child(j)))
				{
					std::string path = menuHead;
					path += "/";
					path += menuItem;

					if (Fl_Menu_Item *item = const_cast<Fl_Menu_Item*>(menu->find_item(path.c_str())))
					{
						if(t.find("disable") != std::string::npos)
						{
							item->deactivate();
						}
						else if(t.find("enable") != std::string::npos)
						{
							item->activate();
						}
						else if(t.find("mark") != std::string::npos)
						{
							if(item->radio()) item->setonly();
						}
						else if(t.find("plain") != std::string::npos)
						{
							if(item->radio()) item->clear();
						}
						else if(t.find("remove") != std::string::npos)
						{
//							item->remove();
						}
						else
							Error(option, "OPTION");
						return;
					}
					Error(menuItem, "MENUITEM");
				}
			}
		}
	}
	Error(view, "VIEW");
}

void YabInterface::SubMenu(const char* menuHead, const char* menuItem, const char* subMenuItem, const char* shortcut, const char* view)
{
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			Fl::lock();
			YabMenuBar *menu;
			if (yabViewList[i]->HasMenu() == false)
			{
				BPoint newCoor = GetWindowCoordinates(yabViewList[i], 0, 0);
				menu = new YabMenuBar((int)newCoor.x, (int)newCoor.y, yabViewList[i]->w(), 20);
				yabViewList[i]->add(menu);
				yabViewList[i]->HasMenu(true);
			}
			else
			{
				for(int j = 0; j < yabViewList[i]->children(); j++)
				{
					if(menu = dynamic_cast<YabMenuBar*>(yabViewList[i]->child(j)))
						break;
				}
			}
			menu->add(menuHead, menuItem, subMenuItem, shortcut, StaticMessageCallback);

			yabViewList[i]->redraw();
			Fl::unlock();
			return;
		}
	}
	Error(view, "VIEW");
}

void YabInterface::SubMenu(const char* menuHead, const char* menuItem, int isRadio, const char* view)
{
	if (isRadio < 1) return;
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			for(int j = 0; j < yabViewList[i]->children(); j++)
			{
				if(YabMenuBar *menu = dynamic_cast<YabMenuBar*>(yabViewList[i]->child(j)))
				{
					bool found1=false;
					bool found2=false;
					for (int j=0; j<menu->size();j++)
					{
						const char* item = menu->text(j);
						if (found1)
						{
							if (found2)
							{
								if (item == NULL) break;
								int m = menu->mode(j);
								menu->mode(j, m|FL_MENU_RADIO);
							}
							else if (item && strcmp(item, menuItem) == 0 && menu->mode(j) == FL_SUBMENU)
								found2 = true;
							if (item == NULL) found1 = false;
						}
						else if (item && strcmp(item, menuHead) == 0 && menu->mode(j) == FL_SUBMENU)
								found1 = true;
					}
					return;
				}
			}
		}
	}
	Error(view, "VIEW");
}

void YabInterface::SubMenu3(const char* menuHead, const char* menuItem, const char* subMenuItem, const char* option, const char* view)
{
	std::string s = view;

	std::string t = option;
	std::transform(t.begin(),t.end(),t.begin(),(int (*)(int))std::tolower);

	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			for(int j = 0; j < yabViewList[i]->children(); j++)
			{
				if(YabMenuBar *menu = dynamic_cast<YabMenuBar*>(yabViewList[i]->child(j)))
				{
					std::string path = menuHead;
					path += "/";
					path += menuItem;
					path += "/";
					path += subMenuItem;

					if (Fl_Menu_Item *item = const_cast<Fl_Menu_Item*>(menu->find_item(path.c_str())))
					{
						if(t.find("disable") != std::string::npos)
						{
							item->deactivate();
						}
						else if(t.find("enable") != std::string::npos)
						{
							item->activate();
						}
						else if(t.find("mark") != std::string::npos)
						{
							if(item->radio()) item->setonly();
						}
						else if(t.find("plain") != std::string::npos)
						{
							if(item->radio()) item->clear();
						}
						else if(t.find("remove") != std::string::npos)
						{
//							item->remove();
						}
						else
							Error(option, "OPTION");
						return;
					}
					Error(menuItem, "MENUITEM");
				}
			}
		}
	}
	Error(view, "VIEW");
}

void YabInterface::CreateTextControl(BRect frame, const char* id, const char* label, const char* text, const char* view)
{
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			int w;

			Fl::lock();

			BPoint newCoor = GetWindowCoordinates(yabViewList[i], frame.x1, frame.y1);
			w = (int)fl_width(label);

			YabTextControl *txt = new YabTextControl((int)newCoor.x+w, (int)newCoor.y, (int)frame.width-w, (int)frame.height, id, label);
			txt->type(FL_NORMAL_INPUT);
			txt->labelsize(B_FONT_SIZE);
			txt->textsize(B_FONT_SIZE);
			txt->value(text);

			txt->when(FL_WHEN_ENTER_KEY);
			txt->callback(StaticMessageCallback);

			yabViewList[i]->add(txt);
			yabViewList[i]->redraw();

			Fl::unlock();

			return;
		}
	}
	Error(view, "VIEW");
}

void YabInterface::TextControl(const char* id, const char* text)
{
	std::string s = id;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if(YabTextControl *control = dynamic_cast<YabTextControl*>(yabViewList[i]->child(j)))
			{
				if(s == control->GetID())
				{
					Fl::lock();
					control->value(text);
					control->redraw();
					Fl::unlock();
					return;
				}
			}
		}
	}
	Error(id, "TEXTCONTROL");
}

void YabInterface::TextControl(const char* id)
{
	std::string s = id;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if(YabTextControl *control = dynamic_cast<YabTextControl*>(yabViewList[i]->child(j)))
			{
				if(s == control->GetID())
				{
					Fl::lock();
					control->value("");
					control->redraw();
					Fl::unlock();
					return;
				}
			}
		}
	}
	Error(id, "TEXTCONTROL");
}

const char* YabInterface::TextControlGet(const char* id)
{
	std::string s = id;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if(YabTextControl *control = dynamic_cast<YabTextControl*>(yabViewList[i]->child(j)))
			{
				if(s == control->GetID())
				{
					Fl::lock();
					const char* ret = control->value();
					Fl::unlock();
					return ret;
				}
			}
		}
	}
	Error(id, "TEXTCONTROL");
}

void YabInterface::TextControl(const char* id, int mode)
{
	std::string s = id;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if(YabTextControl *control = dynamic_cast<YabTextControl*>(yabViewList[i]->child(j)))
			{
				if(s == control->GetID())
				{
					Fl::lock();
					if (mode < 1)
						control->type(FL_NORMAL_INPUT);
					else
						control->type(FL_SECRET_INPUT);
					control->redraw();
					Fl::unlock();
					return;
				}
			}
		}
	}
	Error(id, "TEXTCONTROL");
}

void YabInterface::CreateCheckBox(double x, double y, const char* id, const char* label, int isActivated, const char* view)
{
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			int w;

			Fl::lock();

			BPoint newCoor = GetWindowCoordinates(yabViewList[i], x, y);

			YabCheckButton *check = new YabCheckButton((int)newCoor.x, (int)newCoor.y, id, label);

			check->type(FL_TOGGLE_BUTTON);
			if(isActivated) check->set();
			check->callback(StaticMessageCallback);
			check->color(fl_rgb_color(B_GREY));
			check->labelsize(B_FONT_SIZE);

			w = (int)fl_width(label);
			check->size(w+2*B_FONT_SIZE-4, B_FONT_SIZE*2-4);

			yabViewList[i]->add(check);
			yabViewList[i]->redraw();

			Fl::unlock();

			return;
		}
	}
	Error(view, "VIEW");
}

void YabInterface::CheckboxSet(const char* id, int isActivated)
{
	std::string s = id;
	for (int i = 0; i < yabViewList.size(); i++)
		for(int j = 0; j < yabViewList[i]->children(); j++)
			if(YabCheckButton *check = dynamic_cast<YabCheckButton*>(yabViewList[i]->child(j)))
				if(s == check->GetID())
				{
					check->value(isActivated);
					return;
				}

	Error(id, "CHECKBOX");
}

void YabInterface::CreateRadioButton(double x, double y, const char* id, const char* label, int isActivated, const char* view)
{
	std::string s = view;

	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			int w;

			Fl::lock();

			BPoint newCoor = GetWindowCoordinates(yabViewList[i], x, y);

			YabRadioButton *radio = new YabRadioButton((int)newCoor.x, (int)newCoor.y, id, label);

			radio->type(FL_RADIO_BUTTON);
			if(isActivated) radio->set();
			radio->callback(StaticMessageCallback);
			radio->color(fl_rgb_color(B_GREY));
			radio->labelsize(B_FONT_SIZE);

			w = (int)fl_width(label);
			radio->size(w+2*B_FONT_SIZE-4, B_FONT_SIZE*2-4);

			yabViewList[i]->add(radio);
			yabViewList[i]->redraw();

			Fl::unlock();

			return;
		}
	} 
	Error(view, "VIEW");
}

void YabInterface::RadioSet(const char* id, int isActivated)
{
	std::string s = id;
	for (int i = 0; i < yabViewList.size(); i++)
		for(int j = 0; j < yabViewList[i]->children(); j++)
			if(YabRadioButton *radio = dynamic_cast<YabRadioButton*>(yabViewList[i]->child(j)))
				if(s == radio->GetID())
				{
					radio->value(isActivated);
					return;
				}

	Error(id, "RADIOBUTTON");
}

void YabInterface::CreateListBox(BRect frame, const char* id, int scrollbar, const char* view)
{
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			Fl::lock();

			BPoint newCoor = GetWindowCoordinates(yabViewList[i], frame.x1, frame.y1);

			YabListBox *list = new YabListBox((int)newCoor.x, (int)newCoor.y, (int)frame.width, (int)frame.height, id);
			list->callback(StaticMessageCallback);
			switch(scrollbar)
			{
				case 0: break;
				case 2: list->has_scrollbar(Fl_Browser_::HORIZONTAL_ALWAYS);
					break;
				case 3: list->has_scrollbar(Fl_Browser_::BOTH_ALWAYS);
					break;
				default: list->has_scrollbar(Fl_Browser_::VERTICAL_ALWAYS);
					 break;
			}
			list->labelsize(B_FONT_SIZE);

			yabViewList[i]->add(list);
			yabViewList[i]->redraw();

			Fl::unlock();

			return;
		}
	}
	Error(view, "VIEW");
}

void YabInterface::ClearItems(const char* title)
{
	// This is listbox clear
	std::string id = title;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if(YabListBox *list = dynamic_cast<YabListBox*>(yabViewList[i]->child(j)))
			{
				if(id == list->GetID())
				{
					Fl::lock();
					list->clear();
					list->redraw();
					Fl::unlock();
					return;
				}
			}
		}
	}
	Error(title, "LISTBOX");
}

void YabInterface::ListboxAdd(const char* listbox, const char* item)
{
	std::string s = listbox;
	for (int i = 0; i < yabViewList.size(); i++)
		for(int j = 0; j < yabViewList[i]->children(); j++)
			if(YabListBox *list = dynamic_cast<YabListBox*>(yabViewList[i]->child(j)))
				if(s == list->GetID())
				{
					Fl::lock();
					std::string t = "@S12"; // B_FONT_SIZE
					t += item;
					list->add(t.c_str());
					Fl::unlock();
					return;
				}

	Error(listbox, "LISTBOX");
}

void YabInterface::ListboxAdd(const char* listbox, int pos, const char* item)
{
	std::string s = listbox;
	for (int i = 0; i < yabViewList.size(); i++)
		for(int j = 0; j < yabViewList[i]->children(); j++)
			if(YabListBox *list = dynamic_cast<YabListBox*>(yabViewList[i]->child(j)))
				if(s == list->GetID())
				{
					Fl::lock();
					std::string t = "@S12"; // B_FONT_SIZE
					t += item;
					list->insert(pos, t.c_str());
					Fl::unlock();
					return;
				}

	Error(listbox, "LISTBOX");
}

void YabInterface::ListboxSelect(const char* listbox, int pos)
{
	std::string s = listbox;
	for (int i = 0; i < yabViewList.size(); i++)
		for(int j = 0; j < yabViewList[i]->children(); j++)
			if(YabListBox *list = dynamic_cast<YabListBox*>(yabViewList[i]->child(j)))
				if(s == list->GetID())
				{
					Fl::lock();
					if(pos<0) pos = 0;
					if(pos>list->size()) pos = list->size();
					list->select(pos);
					list->middleline(pos);
					Fl::unlock();
					return;
				}

	Error(listbox, "LISTBOX");
}

void YabInterface::RemoveItem(const char* title, const char* item)
{
	// This is listbox remove
	std::string id = title;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if(YabListBox *list = dynamic_cast<YabListBox*>(yabViewList[i]->child(j)))
			{
				if(id == list->GetID())
				{
					Fl::lock();
					for (int n=0; n<=list->size(); n++)
					{
						const char* entry = list->text(n);
						if (strcmp(entry, item) == 0)
						{
							list->remove(n);
							list->redraw();
							break;
						}
					}
					Fl::unlock();
					return;
				}
			}
		}
	}
	Error(title, "LISTBOX");
}

void YabInterface::ListboxRemove(const char* listbox, int pos)
{
	std::string s = listbox;
	for (int i = 0; i < yabViewList.size(); i++)
		for(int j = 0; j < yabViewList[i]->children(); j++)
			if(YabListBox *list = dynamic_cast<YabListBox*>(yabViewList[i]->child(j)))
				if(s == list->GetID())
				{
					Fl::lock();
					if(pos<0) pos = 0;
					if(pos>list->size()) pos = list->size();
					list->remove(pos);
					Fl::unlock();
					return;
				}

	Error(listbox, "LISTBOX");
}

const char* YabInterface::ListboxGet(const char* listbox, int pos)
{
	std::string s = listbox;
	for (int i = 0; i < yabViewList.size(); i++)
		for(int j = 0; j < yabViewList[i]->children(); j++)
			if(YabListBox *list = dynamic_cast<YabListBox*>(yabViewList[i]->child(j)))
				if(s == list->GetID())
				{
					Fl::lock();
					if(pos<0) pos = 0;
					if(pos>list->size()) pos = list->size();
					const char* ret = list->text(pos);
					Fl::unlock();
					return ret;
				}

	Error(listbox, "LISTBOX");
}

int YabInterface::ListboxCount(const char* listbox)
{
	std::string s = listbox;
	for (int i = 0; i < yabViewList.size(); i++)
		for(int j = 0; j < yabViewList[i]->children(); j++)
			if(YabListBox *list = dynamic_cast<YabListBox*>(yabViewList[i]->child(j)))
				if(s == list->GetID())
					return list->size();

	Error(listbox, "LISTBOX");
}

void YabInterface::CreateDropBox(BRect frame, const char* title, const char* label, const char* view)
{ 
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			Fl::lock();
			BPoint newCoor = GetWindowCoordinates(yabViewList[i], frame.x1, frame.y1);

			YabDropBox *dropbox = new YabDropBox((int)newCoor.x, (int)newCoor.y, (int)frame.width, (int)frame.height, title, label);
			dropbox->callback(StaticMessageCallback);

			yabViewList[i]->add(dropbox);
			yabViewList[i]->redraw();

			Fl::unlock();
			return;
		}
	}
	Error(view, "VIEW");
}

void YabInterface::CreateItem(const char* id, const char* item)
{
	std::string s = id;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if(YabDropBox *db = dynamic_cast<YabDropBox*>(yabViewList[i]->child(j)))
			{
				if(s == db->GetID())
				{
					Fl::lock();
					int z = db->size()-2;
					if (strcmp(item, "--") == 0)
					{
						if (z >=0) db->mode(z, FL_MENU_DIVIDER);
					}
					else
					{
						if (z >=0) db->value(0);
						db->add(item, 0, StaticMessageCallback, (void*)item);
					}
					db->redraw();
					Fl::unlock();
					return;
				}
			}
		}
	}
	Error(id, "DROPBOX");
}

void YabInterface::DropBoxSelect(const char* dropbox, int position)
{
	// printf("-- this command makes no sense in fltk\n");
}

void YabInterface::DropBoxClear(const char* dropbox)
{
	std::string id = dropbox;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if(YabDropBox *db = dynamic_cast<YabDropBox*>(yabViewList[i]->child(j)))
			{
				if(id == db->GetID())
				{
					Fl::lock();
					db->clear();
					Fl::unlock();
					db->redraw();
					return;
				}
			}
		}
	}
	Error(dropbox, "DROPBOX");
}

void YabInterface::DropBoxRemove(const char* dropbox, int position)
{
	std::string id = dropbox;
	int p = position-1;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if(YabDropBox *db = dynamic_cast<YabDropBox*>(yabViewList[i]->child(j)))
			{
				if(id == db->GetID())
				{
					int s = db->size()-1;
					if(s >= 0 && p <= s && p >= 0)
					{
						Fl::lock();
						db->remove(p);
						Fl::unlock();
						db->redraw();
					}
					return;
				}
			}
		}
	}
	Error(dropbox, "DROPBOX");
}

int YabInterface::DropBoxCount(const char* dropbox)
{
	std::string id = dropbox;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if(YabDropBox *db = dynamic_cast<YabDropBox*>(yabViewList[i]->child(j)))
			{
				if(id == db->GetID())
				{
					return db->size()-1;
				}
			}
		}
	}
	Error(dropbox, "DROPBOX");
}

const char* YabInterface::DropBoxGet(const char* dropbox, int position)
{
	std::string id = dropbox;
	int p = position-1;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if(YabDropBox *db = dynamic_cast<YabDropBox*>(yabViewList[i]->child(j)))
			{
				if(id == db->GetID())
				{
					int s = db->size()-1;
					if(s >= 0 && p <= s && p >= 0)
						return db->text(position-1);
					return "";
				}
			}
		}
	}
	Error(dropbox, "DROPBOX");
}

void YabInterface::DrawText(BPoint coordinates, const char* text, const char* window)
{
	std::string v = window;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(v == yabViewList[i]->GetID())
		{
			Fl::lock();
			BPoint newCoor = GetWindowCoordinates(yabViewList[i], coordinates.x, coordinates.y);
			YabDrawing *t = new YabDrawing();
			t->command = 0;
			t->x1 = (int)newCoor.x;
			t->y1 = (int)newCoor.y;
			t->chardata = strdup(text);

			yabViewList[i]->AddDrawing(t);
			yabViewList[i]->redraw();
			Fl::unlock();
			return;
		}
	}
	Error(window, "VIEW or WINDOW");
}

void YabInterface::DrawRect(BRect frame, const char* window)
{
	std::string v = window;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(v == yabViewList[i]->GetID())
		{
			Fl::lock();
			BPoint newCoor1 = GetWindowCoordinates(yabViewList[i], frame.x1, frame.y1);
			BPoint newCoor2 = GetWindowCoordinates(yabViewList[i], frame.x2, frame.y2);

			YabDrawing *t = new YabDrawing();
			if(drawStroking)
				t->command = 4;
			else
				t->command = 5;
			t->x1 = (int)newCoor1.x;
			t->y1 = (int)newCoor1.y;
			t->x2 = (int)newCoor2.x;
			t->y2 = (int)newCoor2.y;

			yabViewList[i]->AddDrawing(t);
			yabViewList[i]->redraw();

			Fl::unlock();
			return;
		}
	}
	Error(window, "VIEW or WINDOW");
}

void YabInterface::DrawClear(const char* window, bool isExit)
{
	std::string v = window;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(v == yabViewList[i]->GetID())
		{
			Fl::lock();
			yabViewList[i]->FlushDrawings();
			/*
			BPoint newCoor1 = GetWindowCoordinates(yabViewList[i], 0,0);
			BPoint newCoor2 = GetWindowCoordinates(yabViewList[i], yabViewList[i]->w() ,yabViewList[i]->h());
			yabViewList[i]->DrawBox((int)newCoor1.x, (int)newCoor1.y, (int)newCoor2.x, (int)newCoor2.y);
			*/

			yabViewList[i]->redraw();
			Fl::unlock();
			return;
		}
	}
	Error(window, "VIEW or WINDOW");
}

void YabInterface::DrawDot(double x, double y, const char* window)
{
	std::string v = window;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(v == yabViewList[i]->GetID())
		{
			Fl::lock();
			BPoint newCoor1 = GetWindowCoordinates(yabViewList[i], x, y);

			YabDrawing *t = new YabDrawing();
			t->command = 1;
			t->x1 = (int)newCoor1.x;
			t->y1 = (int)newCoor1.y;
			t->x2 = (int)newCoor1.x;
			t->y2 = (int)newCoor1.y;

			yabViewList[i]->AddDrawing(t);
			yabViewList[i]->redraw();

			Fl::unlock();
			return;
		}
	}
	Error(window, "VIEW or WINDOW");
}

void YabInterface::DrawLine(double x1, double y1, double x2, double y2, const char* window)
{
	std::string v = window;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(v == yabViewList[i]->GetID())
		{
			Fl::lock();
			BPoint newCoor1 = GetWindowCoordinates(yabViewList[i], x1, y1);
			BPoint newCoor2 = GetWindowCoordinates(yabViewList[i], x2, y2);

			YabDrawing *t = new YabDrawing();
			t->command = 1;
			t->x1 = (int)newCoor1.x;
			t->y1 = (int)newCoor1.y;
			t->x2 = (int)newCoor2.x;
			t->y2 = (int)newCoor2.y;

			yabViewList[i]->AddDrawing(t);
			yabViewList[i]->redraw();

			Fl::unlock();
			return;
		}
	}
	Error(window, "VIEW or WINDOW");
}

void YabInterface::DrawCircle(double x, double y, double r, const char* window)
{
	std::string v = window;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(v == yabViewList[i]->GetID())
		{
			Fl::lock();
			BPoint newCoor = GetWindowCoordinates(yabViewList[i], x, y);

			YabDrawing *t = new YabDrawing();
			if(drawStroking)
				t->command = 2;
			else
				t->command = 3;
			t->x1 = (int)newCoor.x;
			t->y1 = (int)newCoor.y;
			t->x2 = (int)r;
			t->y2 = (int)r;

			yabViewList[i]->AddDrawing(t);
			yabViewList[i]->redraw();

			Fl::unlock();
			return;
		}
	}
	Error(window, "VIEW or WINDOW");
}

void YabInterface::DrawEllipse(double x, double y, double r1, double r2, const char* window)
{
	std::string v = window;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(v == yabViewList[i]->GetID())
		{
			Fl::lock();
			BPoint newCoor = GetWindowCoordinates(yabViewList[i], x, y);

			YabDrawing *t = new YabDrawing();
			if(drawStroking)
				t->command = 2;
			else
				t->command = 3;
			t->x1 = (int)newCoor.x;
			t->y1 = (int)newCoor.y;
			t->x2 = (int)r1;
			t->y2 = (int)r2;

			yabViewList[i]->AddDrawing(t);
			yabViewList[i]->redraw();

			Fl::unlock();
			return;
		}
	}
	Error(window, "VIEW or WINDOW");
}

void YabInterface::DrawCurve(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, const char* window)
{
	std::string v = window;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(v == yabViewList[i]->GetID())
		{
			Fl::lock();
			BPoint newCoor1 = GetWindowCoordinates(yabViewList[i], x1, y1);
			BPoint newCoor2 = GetWindowCoordinates(yabViewList[i], x2, y2);
			BPoint newCoor3 = GetWindowCoordinates(yabViewList[i], x3, y3);
			BPoint newCoor4 = GetWindowCoordinates(yabViewList[i], x4, y4);

			YabDrawing *t = new YabDrawing();
			if(drawStroking)
				t->command = 8;
			else
				t->command = 9;
			t->x1 = (int)newCoor1.x;
			t->y1 = (int)newCoor1.y;
			t->x2 = (int)newCoor2.x;
			t->y2 = (int)newCoor2.y;
			t->x3 = (int)newCoor3.x;
			t->y3 = (int)newCoor3.y;
			t->x4 = (int)newCoor4.x;
			t->y4 = (int)newCoor4.y;

			yabViewList[i]->AddDrawing(t);
			yabViewList[i]->redraw();

			Fl::unlock();
			return;
		}
	}
	Error(window, "VIEW or WINDOW");
}

void YabInterface::CreateText(double x, double y, const char* id, const char* text, const char* view)
{
}

void YabInterface::Text2(BRect frame, const char* id, const char* text, const char* view)
{
}

void YabInterface::TextAlign(const char* txt, const char* option)
{
}

void YabInterface::Slider(BRect frame, const char* id, const char* title, int min, int max, const char* view)
{
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			Fl::lock();

			BPoint newCoor = GetWindowCoordinates(yabViewList[i], frame.x1, frame.y1);

			int x = static_cast<int>(newCoor.x);
			int y = static_cast<int>(newCoor.y);
			int w = static_cast<int>(frame.width);

			YabSlider *yabslider = new YabSlider(x, y, w, 20, id, title);
			yabslider->type(FL_HORIZONTAL);	//type(FL_HOR_NICE_SLIDER);

			yabslider->LabelLeft();
			yabslider->minimum(min);
			yabslider->maximum(max);
			yabslider->callback(StaticMessageCallback);
			yabslider->redraw();

			yabViewList[i]->add(yabslider);
			yabViewList[i]->redraw();

			Fl::unlock();

			return;
		}
	} 
	Error(view, "VIEW");
}

void YabInterface::Slider(BRect frame, const char* id, const char* title, int min, int max, const char* option, const char* view)
{
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			Fl::lock();
			BPoint newCoor = GetWindowCoordinates(yabViewList[i], frame.x1, frame.y1);

			YabSlider *yabslider;
			std::string s = option;
			if (s == "vertical")
			{
				yabslider = new YabSlider((int)newCoor.x, (int)newCoor.y, 20, (int)frame.height, id,title);
				yabslider->type(FL_VERTICAL);
				yabslider->LabelTop();
			}
			else
			{
				yabslider = new YabSlider((int)newCoor.x, (int)newCoor.y, (int)frame.width, 20, id,title);
				yabslider->type(FL_HORIZONTAL);
				yabslider->LabelLeft();
				yabslider->slider(FL_DIAMOND_DOWN_BOX);
				yabslider->selection_color(2);
			}
			
			yabslider->minimum(min);
			yabslider->maximum(max);
			yabslider->callback(StaticMessageCallback);
			yabslider->redraw();
			
			yabViewList[i]->add(yabslider);
			yabViewList[i]->redraw();

			Fl::unlock();

			return;
		}
	} 
	Error(view, "VIEW");
}

void YabInterface::SetSlider(const char* id, const char* label1, const char* label2)
{
	std::string s = id;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if(YabSlider *slider = dynamic_cast<YabSlider*>(yabViewList[i]->child(j)))
			{
				if(s == slider->GetID())
				{
					Fl::lock();
					slider->copy_label(label1);
					slider->redraw_label();
					Fl::unlock();
					return;
				}
			}
		}
	}
	Error(id, "SLIDER");
}

void YabInterface::SetSlider(const char* id, const char* bottomtop, int count)
{
}

void YabInterface::SetSlider(const char* id, const char* part, int r, int g, int b)
{
	int myMode = 0;

	std::string t = part;
	std::transform(t.begin(),t.end(),t.begin(),(int (*)(int))std::tolower);

	if(t.find("barcolor") != std::string::npos) myMode = 1;
	if(t.find("fillcolor") != std::string::npos) myMode = 2;
	if (myMode == 0) Error(part, "PART");

	std::string s = id;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if(YabSlider *slider = dynamic_cast<YabSlider*>(yabViewList[i]->child(j)))
			{
				if(s == slider->GetID())
				{
					Fl::lock();
					if (myMode == 1)
						slider->color(fl_rgb_color(r, g, b));
					else if (myMode ==2)
						slider->selection_color(fl_rgb_color(r, g, b));
					slider->redraw();
					Fl::unlock();
					return;
				}
			}
		}
	}
	Error(id, "SLIDER");
}

void YabInterface::SetSlider(const char* id, int value)
{
	std::string s = id;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if(YabSlider *slider = dynamic_cast<YabSlider*>(yabViewList[i]->child(j)))
			{
				if(s == slider->GetID())
				{
					Fl::lock();
					double v = slider->round(value);
					slider->value(v);
					slider->redraw();
					Fl::unlock();
					return;
				}
			}
		}
	}
	Error(id, "SLIDER");
}

int YabInterface::SliderGet(const char* slider)
{
	std::string s = slider;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if(YabSlider *slider = dynamic_cast<YabSlider*>(yabViewList[i]->child(j)))
			{
				if(s == slider->GetID())
				{
					return static_cast<int>(slider->value());
				}
			}
		}
	}
	Error(slider, "SLIDER");
}

void YabInterface::SetOption(const char* id, const char* option, const char* value)
{
}

void YabInterface::SetOption(const char* id, const char* option, int r, int g, int b)
{
}

void YabInterface::SetOption(const char* id, const char* option)
{
}

void YabInterface::SetOption(const char* id, const char* option, int value)
{
}

void YabInterface::DropZone(const char* view)
{
}

void YabInterface::ColorControl(double x, double y, const char* id, const char* view)
{
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			Fl::lock();
			BPoint point = GetWindowCoordinates(yabViewList[i], x, y);

			YabColorControl *control = new YabColorControl((int)point.x, (int)point.y, 276, 54, id);
			control->rgb(0, 0, 0);
			control->redraw();

			yabViewList[i]->add(control);
			yabViewList[i]->redraw();
			Fl::unlock();
			return;
		}
	}
	Error(view, "VIEW");
}

void YabInterface::ColorControl(const char* id, int r, int g, int b)
{
	std::string s = id;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if(YabColorControl *control = dynamic_cast<YabColorControl*>(yabViewList[i]->child(j)))
			{
				if(s == control->GetID())
				{
					Fl::lock();
					control->rgb(r, g, b);
					control->redraw();
					Fl::unlock();
					return;
				}
			}
		}
	}
	Error(id, "COLORCONTROL");
}

int YabInterface::ColorControlGet(const char* colorcontrol, const char* option)
{
	int myMode = 0;

	std::string t = option;
	std::transform(t.begin(),t.end(),t.begin(),(int (*)(int))std::tolower);

	if(t.find("red") != std::string::npos) myMode = 1;
	if(t.find("green") != std::string::npos) myMode = 2;
	if(t.find("blue") != std::string::npos) myMode = 3;
	if(myMode == 0) Error(option, "OPTION");

	std::string s = colorcontrol;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		for(int j = 0; j < yabViewList[i]->children(); j++)
		{
			if(YabColorControl *control = dynamic_cast<YabColorControl*>(yabViewList[i]->child(j)))
			{
				if(s == control->GetID())
				{
					int ret;
					Fl::lock();
					if(myMode == 1) ret = (int)control->r();
					if(myMode == 2) ret = (int)control->g();
					if(myMode == 3) ret = (int)control->b();
					Fl::unlock();
					return ret;
				}
			}
		}
	}
	Error(colorcontrol, "COLORCONTROL");
}

void YabInterface::CreateAlert(const char* text, const char* button1, const char* option)
{
	YabAlert *alert = new YabAlert(text, button1, "", "", option);
	if(alert->HasType() == -1)
		ErrorGen("Invalid option");

	int pressed = alert->ARun();
	delete alert;
	return;
}

int YabInterface::NewAlert(const char* text, const char* button1, const char* button2, const char* button3, const char* option)
{
	YabAlert *alert = new YabAlert(text, button1, button2, button3, option);
	if(alert->HasType() == -1)
		ErrorGen("Invalid option");

	int pressed = alert->ARun();
	delete alert;
	return pressed;
}

const char* YabInterface::FilePanel(const char *mode, const char* title, const char* directory, const char* filename)
{
	int type=0;

	std::string t = mode;
	std::transform(t.begin(),t.end(),t.begin(),(int (*)(int))std::tolower);

	if(t.find("load-file") != std::string::npos)
	{
		type = Fl_File_Chooser::SINGLE;
	}
	else if(t.find("save-file") != std::string::npos)
	{
		type = Fl_File_Chooser::CREATE;
	}
	else if(t.find("load-directory") != std::string::npos)
	{
		type = Fl_File_Chooser::DIRECTORY;
	}
	else if(t.find("load-file-and-directory") == 0)
	{
		type = Fl_File_Chooser::SINGLE;
	}
	else
		ErrorGen("Invalid Option");

	std::string result="";
	Fl::lock();
	Fl_File_Chooser *chooser = new Fl_File_Chooser(directory, "*", type, title);
	chooser->preview(0);
	chooser->show();
	Fl::unlock();

	while (chooser->shown())
	{
		Snooze(0.1);
	}

	if (chooser->value() != NULL)
		result = chooser->value();
	delete chooser;
	return result.c_str();
}

void YabInterface::SetLayout(const char* layout, const char* window) 
{
}

/*
void YabInterface::RemoveView(BView *myView)
{
}
*/

void YabInterface::TextEdit(BRect frame, const char* title, int scrollbar, const char* window)
{
}

void YabInterface::TextAdd(const char* title, const char* text)
{
}

void YabInterface::TextSet(const char* title, const char* option)
{
}

void YabInterface::TextSet(const char* title, const char* option, const char* value)
{
}

void YabInterface::TextSet(const char* title, const char* option, int value)
{
}

void YabInterface::TextColor(const char* title, const char* option, const char* command)
{
}

void YabInterface::TextColor(const char* title, const char* option, int r, int g, int b)
{
}

int YabInterface::TextGet(const char* title, const char* option, const char* option2)
{
}

double YabInterface::TextGet(const char* title, const char* option, int line)
{
}

int YabInterface::TextGet(const char* title, const char* option)
{
}

const char* YabInterface::TextGet(const char* title, int linenum)
{
}

double YabInterface::DrawGet(const char* option, const char* txt, const char* view)
{
}

const char* YabInterface::DrawGet(const char* option)
{
}

int YabInterface::DrawGet(BPoint coord, const char* option, const char* view)
{
}

void YabInterface::TextClear(const char* title)
{
}

const char* YabInterface::TextGet(const char* title)
{
}

void YabInterface::TreeBox1(BRect frame, const char* id, int scrollbarType, const char* view)
{
}

void YabInterface::TreeBox2(const char* id, const char* item)
{
}

void YabInterface::TreeBox3(const char* id, const char* head, const char* item, int isExpanded)
{
}

void YabInterface::TreeBox4(const char* id)
{
}

void YabInterface::TreeBox5(const char* id, const char* item)
{
}

void YabInterface::TreeBox7(const char* id, int pos)
{
}

void YabInterface::TreeBox8(const char* id, int pos)
{
}

void YabInterface::TreeBox9(const char* id, const char* head, const char* item)
{
}

void YabInterface::TreeBox10(const char* id, const char* head)
{
}

void YabInterface::TreeBox11(const char* id, const char* head)
{
}

void YabInterface::TreeBox12(const char* id, const char* item, int pos)
{
}

const char* YabInterface::TreeboxGet(const char* treebox, int pos)
{
}

int YabInterface::TreeboxCount(const char* treebox)
{
}

/*BBitmap* YabInterface::loadImage(const char* FileName)
{
}*/

void YabInterface::ButtonImage(double x,double y, const char* id,const char* enabledon, const char* enabledoff, const char* disabled, const char* view)
{
}

void YabInterface::CheckboxImage(double x, double y,const char* id,const char* enabledon, const char* enabledoff, const char *disabledon, const char *disabledoff, int isActivated, const char* view)
{
}

void YabInterface::ToolTips(const char* view, const char* text)
{
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			char* t = (char*)malloc(sizeof(text));
			strcpy(t, text);
			tooltips.push_back(t);
			yabViewList[i]->tooltip(t);
			return;
		}
		for(int j = 0; j < yabViewList[i]->children(); j++)
			if(YabWidget *widget = dynamic_cast<YabWidget*>(yabViewList[i]->child(j)))
				if(s == widget->GetID())
					if(Fl_Widget *fl = dynamic_cast<Fl_Widget*>(yabViewList[i]->child(j)))
					{
						char* t = (char*)malloc(sizeof(text));
						strcpy(t, text);
						tooltips.push_back(t);
						fl->tooltip(t);
						return;
					}
	}
	Error(view, "VIEW");
}

void YabInterface::ToolTipsColor(const char* color, int r, int g, int b)
{
	// does nothing in flyab
}

void YabInterface::TreeSort(const char* view)
{
}

void YabInterface::ListSort(const char* view)
{
}

/*int YabInterface::compare(BListItem **firstArg, BListItem **secondArg)
{
	if(firstArg != NULL && secondArg != NULL)
	{
		BString item1(((BStringItem*)*firstArg)->Text());
		BString item2(((BStringItem*)*secondArg)->Text());
		if(((BListItem*)*firstArg)->OutlineLevel()!=((BListItem*)*secondArg)->OutlineLevel())
			return 0;
		return item1.ICompare(item2);
	}
	return 0;
} */

void YabInterface::FileBox(BRect frame, const char* id, bool hasHScrollbar, const char* option, const char* view)
{
}

void YabInterface::ColumnBoxAdd(const char* id, int column, int position, int height, const char* text)
{
}

void YabInterface::FileBoxAdd(const char* columnbox, const char* name, int pos, double minWidth, double maxWidth, double width, const char* option)
{
}

void YabInterface::FileBoxClear(const char* columnbox)
{
}

void YabInterface::ColumnBoxRemove(const char* columnbox, int position)
{
}

void YabInterface::ColumnBoxColor(const char* columnbox, const char* option, int r, int g, int b)
{
}

void YabInterface::ColumnBoxSelect(const char* columnbox, int position)
{
}

const char* YabInterface::ColumnBoxGet(const char* columnbox, int column, int position)
{
}

int YabInterface::ColumnBoxCount(const char* columnbox)
{
}

void YabInterface::DrawSet1(const char* option, const char* window)
{
	int myFont = -1;
	int fontSize = B_FONT_SIZE;

	std::string t = option;
	std::transform(t.begin(),t.end(),t.begin(),(int (*)(int))std::tolower);

	// currently only fixed fonts
	if(t.find("system-plain", 0) != std::string::npos)
		myFont = FL_HELVETICA;
	else if(t.find("system-fixed", 0) != std::string::npos)
		myFont = FL_SCREEN;
	else if(t.find("system-bold", 0) != std::string::npos)
		myFont = FL_HELVETICA_BOLD;
	else if(t.find("helvetica", 0) != std::string::npos && t.find("bold", 0) != std::string::npos && t.find("italic", 0) != std::string::npos)
		myFont = FL_HELVETICA_BOLD_ITALIC;
	else if(t.find("helvetica", 0) != std::string::npos && t.find("bold", 0) != std::string::npos)
		myFont = FL_HELVETICA_BOLD;
	else if(t.find("helvetica", 0) != std::string::npos && t.find("italic", 0) != std::string::npos)
		myFont = FL_HELVETICA_ITALIC;
	else if(t.find("helvetica", 0) != std::string::npos)
		myFont = FL_HELVETICA;
	else if(t.find("courier", 0) != std::string::npos && t.find("bold", 0) != std::string::npos && t.find("italic", 0) != std::string::npos)
		myFont = FL_COURIER_BOLD_ITALIC;
	else if(t.find("courier", 0) != std::string::npos && t.find("bold", 0) != std::string::npos)
		myFont = FL_COURIER_BOLD;
	else if(t.find("courier", 0) != std::string::npos && t.find("italic", 0) != std::string::npos)
		myFont = FL_COURIER_ITALIC;
	else if(t.find("courier", 0) != std::string::npos)
		myFont = FL_COURIER;
	else if(t.find("times", 0) != std::string::npos && t.find("bold", 0) != std::string::npos && t.find("italic", 0) != std::string::npos)
		myFont = FL_TIMES_BOLD_ITALIC;
	else if(t.find("times", 0) != std::string::npos && t.find("bold", 0) != std::string::npos)
		myFont = FL_TIMES_BOLD;
	else if(t.find("times", 0) != std::string::npos && t.find("italic", 0) != std::string::npos)
		myFont = FL_TIMES_ITALIC;
	else if(t.find("times", 0) != std::string::npos)
		myFont = FL_TIMES;
	else if(t.find("symbol", 0) != std::string::npos)
		myFont = FL_SYMBOL;
	else if(t.find("screen", 0) != std::string::npos && t.find("bold", 0) != std::string::npos)
		myFont = FL_SCREEN_BOLD;
	else if(t.find("screen", 0) != std::string::npos)
		myFont = FL_SCREEN;
	else if(t.find("zapf-dingbats", 0) != std::string::npos)
		myFont = FL_ZAPF_DINGBATS;
	// map some popular Zeta fonts
	else if(t.find("zurich", 0) != std::string::npos && t.find("bold", 0) != std::string::npos && t.find("italic", 0) != std::string::npos)
		myFont = FL_HELVETICA_BOLD_ITALIC;
	else if(t.find("zurich", 0) != std::string::npos && t.find("bold", 0) != std::string::npos)
		myFont = FL_HELVETICA_BOLD;
	else if(t.find("zurich", 0) != std::string::npos && t.find("italic", 0) != std::string::npos)
		myFont = FL_HELVETICA_ITALIC;
	else if(t.find("zurich", 0) != std::string::npos)
		myFont = FL_HELVETICA;
	else if(t.find("swis721 bt", 0) != std::string::npos && t.find("bold", 0) != std::string::npos && t.find("italic", 0) != std::string::npos)
		myFont = FL_TIMES_BOLD_ITALIC;
	else if(t.find("swis721 bt", 0) != std::string::npos && t.find("bold", 0) != std::string::npos)
		myFont = FL_TIMES_BOLD;
	else if(t.find("swis721 bt", 0) != std::string::npos && t.find("italic", 0) != std::string::npos)
		myFont = FL_TIMES_ITALIC;
	else if(t.find("swis721 bt", 0) != std::string::npos)
		myFont = FL_TIMES;
	else return; // Error message?

	// font size
	int pos = t.rfind(',', t.length());
	if(pos != std::string::npos)
	{
		fontSize = atoi((t.substr(pos+1)).c_str());
		if(fontSize == 0) fontSize = B_FONT_SIZE;
	}
	
	// find the view
	std::string v = window;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(v == yabViewList[i]->GetID())
		{
			Fl::lock();

			YabDrawing *t = new YabDrawing();
			t->command = 12;
			t->x1 = myFont;
			t->y1 = fontSize;

			yabViewList[i]->AddDrawing(t);
			yabViewList[i]->redraw();

			Fl::unlock();
			return;
		}
	}
	Error(window, "VIEW or WINDOW");
}

void YabInterface::DrawSet2(int fillorstroke, const char* mypattern)
{
	drawStroking = fillorstroke ? true : false;

	std::string t = mypattern;
	std::transform(t.begin(),t.end(),t.begin(),(int (*)(int))std::tolower);

	if(t.find("highsolidfill",0) != std::string::npos)
		;
	else if(t.find("lowsolidfill",0) != std::string::npos)
		;
	else if(t.find("checkeredfill",0) != std::string::npos)
		;
}

int YabInterface::DeskbarParam(const char* option)
{
}

int YabInterface::DesktopParam(bool isWidth)
{
}

void YabInterface::ClipboardCopy(const char* text)
{
}

int YabInterface::Printer(const char* docname, const char *config, const char* view)
{
}

void YabInterface::PrinterConfig(const char* config)
{
}

const char* YabInterface::ClipboardPaste()
{
}

void YabInterface::Calendar(double x, double y, const char* id, const char* format, const char* date, const char* view)
{
}

const char* YabInterface::Calendar(const char* id)
{
}

void YabInterface::Calendar(const char* id, const char* date)
{
}

void YabInterface::Scrollbar(const char* id, int format, const char* view)
{
}

void YabInterface::ScrollbarSet(const char* scrollview, const char* option, double position)
{
}

void YabInterface::ScrollbarSet(const char* scrollview, const char* option, double opt1, double opt2)
{
}

double YabInterface::ScrollbarGet(const char* scrollview, const char* option)
{
}

void YabInterface::SplitView(BRect frame, const char* id, int isVertical, int style, const char* view)
{
}

void YabInterface::SplitView(const char* splitView, const char* option, double position)
{
}

void YabInterface::SplitView(const char* splitView, const char* option, double left, double right)
{
}

double YabInterface::SplitViewGet(const char* splitView, const char* option)
{
}

void YabInterface::DrawSet3(const char* option, int transparency)
{
}

void YabInterface::TextURL(double x, double y, const char* id, const char* text, const char* url, const char* view)
{
}

void YabInterface::TextURL(const char* id, const char* option, int r, int g, int b)
{
}

void YabInterface::SpinControl(double x, double y, const char* id, const char* label, int min, int max, int step, const char* view)
{
	std::string s = view;
	for (int i = 0; i < yabViewList.size(); i++)
	{
		if(s == yabViewList[i]->GetID())
		{
			Fl::lock();

			BPoint newCoor = GetWindowCoordinates(yabViewList[i], x, y);

			int w = (int)fl_width(label);

			YabSpinControl *spin = new YabSpinControl((int)newCoor.x+w,(int)newCoor.y, id, label);
			
			spin->value(min);
			spin->range(min,max);
			spin->step(step);
			
			spin->callback(StaticMessageCallback);

			spin->color(fl_rgb_color(B_GREY));
			spin->labelsize(B_FONT_SIZE);
			spin->textsize(B_FONT_SIZE);

			yabViewList[i]->add(spin);

			yabViewList[i]->redraw();

			Fl::unlock();

			return;
		}
	} 
	Error(view, "VIEW");
}


void YabInterface::SpinControl(const char* spinControl, int value)
{
}

int YabInterface::SpinControlGet(const char *spinControl)
{
}

const char* YabInterface::PopUpMenu(double x, double y, const char* menuItems, const char* view)
{
}

double YabInterface::MenuHeight()
{
}

double YabInterface::TabHeight()
{
}

double YabInterface::ScrollbarWidth()
{
}

const int YabInterface::IsMouseIn(const char* view)
{
}

const char* GetMMsgInfo(int mouseStateInfo, int mouseLButton, int mouseMButton, int mouseRButton, const char* name)
{
}

const char* YabInterface::GetMouseIn()
{
}

const char* YabInterface::KeyboardMessages(const char* view)
{
}

const char* YabInterface::GetMouseMessages(const char* view)
{
}

int YabInterface::ThreadKill(const char* option, int id)
{
}

int YabInterface::ThreadGet(const char* option, const char* appname)
{
}

void YabInterface::Bitmap(double w, double h, const char* id)
{
}

void YabInterface::BitmapDraw(double x, double y, const char* bitmap, const char* mode, const char* view)
{
}

void YabInterface::BitmapDraw(BRect frame, const char* bitmap, const char* mode, const char* view)
{
}

void YabInterface::BitmapGet(BRect frame, const char* id, const char* bitmap)
{
}

void YabInterface::BitmapGet(double w, const char* id, const char* path)
{
}

void YabInterface::BitmapGetIcon(const char* id, const char* option, const char* path)
{
}

void YabInterface::BitmapDrag(const char* bitmap)
{
}

void YabInterface::BitmapRemove(const char* bitmap)
{
}

void YabInterface::Screenshot(BRect frame, const char* bitmap)
{
}

int YabInterface::BitmapSave(const char* id, const char* filename, const char* type)
{
}

void YabInterface::Canvas(BRect frame, const char* id, const char* view)
{
}

int YabInterface::Sound(const char* filename)
{
}

void YabInterface::SoundStop(int id)
{
}

void YabInterface::SoundWait(int id)
{
}

void YabInterface::SetOption(const char* id, const char* option, double x, double y)
{
}

void YabInterface::DrawSet(const char* option, const char* color,const char* view)
{
}

void YabInterface::Treebox13(const char* id,const char* option, int pos)
{
}

int YabInterface::TreeboxGetOpt(const char* id, const char* option, int pos)
{
}

int YabInterface::ListboxGetNum(const char* id)
{
}

int YabInterface::DropboxGetNum(const char* id)
{
}

int YabInterface::TreeboxGetNum(const char* id)
{
}

int YabInterface::ColumnboxGetNum(const char* id)
{
}

void YabInterface::ShortCut(const char* view, const char* key, const char* msg)
{
}

int YabInterface::IsComputerOn()
{
	return 1;
}

void YabInterface::MouseSet(const char* opt)
{
}

void YabInterface::Snooze(double tt)
{
	timespec t;
	t.tv_nsec = (long)(tt * 1000000);
	t.tv_sec = 0;
	nanosleep(&t, NULL);

	/*
	struct timeval tv;
	tv.tv_sec=(long)tt;
	tv.tv_usec=(long)(tt - (int)tt)*1000000;
	select(0,NULL,NULL,NULL,&tv);
	*/
}

const char* YabInterface::GetMessageString()
{
	Snooze(0.01);

	std::string tmp = "";
	if(exiting)
	{
		tmp += "_QuitRequested|";
		exiting = false;
	}
	tmp += localMessage;
	localMessage = "";
	// if(tmp.Length()>32766)
	// 	tmp.Remove(32767, tmp.Length()-32766);
	strcpy(messagebuffer, tmp.c_str());
	return (char*)messagebuffer;
}

int YabInterface::MessageSend(const char* app, const char* msg)
{
}

void YabInterface::SetLocalize(const char* path)
{
}

const int YabInterface::GetErrorCode()
{
	return errorCode;
}

void YabInterface::KillThread(int code)
{
	errorCode = code;
	quitting = true;
	ExitRequested();
	// BMessenger(be_app).SendMessage(new BMessage(B_QUIT_REQUESTED));
	// while(1){}
}

void YabInterface::Error(const char* id, const char* type)
{
	fprintf(stderr, "---Error in %s, line %d: \"%s\" is not of type %s\n", currentLib.c_str(), currentLineNumber, id, type);
	fprintf(stderr,"---Error: Program stopped due to an error \n");
	KillThread(-1);
	// while(1){}
}

void YabInterface::ErrorGen(const char* msg)
{
	fprintf(stderr, "---Error in %s, line %d: %s\n", currentLib.c_str(), currentLineNumber, msg);
	fprintf(stderr,"---Error: Program stopped due to an error \n");
	KillThread(-1);
	// while(1){}
}

void YabInterface::SetCurrentLineNumber(int line, const char* libname)
{
	currentLineNumber = line;

	if(!strcmp(libname, "main"))
		currentLib = mainFileName;
	else
		currentLib = libname;
}

void YabInterface::SetMainFileName(const char* name)
{
	mainFileName = strdup(name);
}

/**
 * C interface functions
 */

const char* yi_GetApplicationDirectory(YabInterface *yab)
{
	return yab->GetApplicationDirectory();
}

void yi_OpenWindow(double x1,double y1,double x2,double y2, const char* id, const char* title, YabInterface* yab)
{
	yab->OpenWindow(BRect(x1,y1,x2,y2), id, _L(title));
}

int yi_CloseWindow(const char* view, YabInterface* yab)
{
	return yab->CloseWindow(view);
}

void yi_CreateButton(double x1,double y1,double x2,double y2, const char* id, const char* title, const char* view, YabInterface* yab) 
{
	yab->CreateButton(BRect(x1,y1,x2,y2), id, _L(title), view);
}

int yi_CreateImage(double x,double y,const char* imagefile, const char* window, YabInterface* yab) 
{
	return yab->CreateImage(BPoint(x,y),imagefile,window);
}

int yi_CreateImage2(double x1,double y1,double x2,double y2,const char* imagefile, const char* window, YabInterface* yab) 
{
	return yab->CreateImage(BRect(x1,y1,x2,y2),imagefile,window);
}

int yi_CreateSVG(double x1,double y1,double x2,double y2,const char* imagefile, const char* window, YabInterface* yab) 
{
	return yab->CreateSVG(BRect(x1,y1,x2,y2),imagefile,window);
}

void yi_CreateMenu(const char* menuhead, const char* menuitem, const char *shortcut, const char* window, YabInterface* yab) 
{
	yab->CreateMenu(_L(menuhead),_L(menuitem),shortcut,window);
}

void yi_CreateTextControl(double x1, double y1, double x2, double y2, const char* id, const char* label, const char* text, const char* window, YabInterface *yab)
{
	yab->CreateTextControl(BRect(x1,y1,x2,y2),id,_L(label),_L(text),window);
}

void yi_CreateCheckBox(double x, double y, const char* id, const char* label, int isActivated, const char* window, YabInterface *yab)
{
	yab->CreateCheckBox(x,y,id,_L(label),isActivated,window);
}

void yi_CreateRadioButton(double x, double y, const char* groupID, const char* label, int isActivated, const char* window, YabInterface *yab)
{
	yab->CreateRadioButton(x,y,groupID,_L(label),isActivated,window);
}

void yi_CreateListBox(double x1, double y1, double x2, double y2, const char* title, int scrollbar, const char* window, YabInterface *yab)
{
	yab->CreateListBox(BRect(x1,y1,x2,y2),title,scrollbar,window); 
}

void yi_CreateDropBox(double x1, double y1, double x2, double y2, const char* title, const char* label, const char* window, YabInterface *yab)
{
	yab->CreateDropBox(BRect(x1,y1,x2,y2),title,_L(label), window);
}

void yi_CreateItem(const char* id,const char* item, YabInterface *yab)
{
	yab->CreateItem(id,_L(item));
}

void yi_RemoveItem(const char* title,const char* item, YabInterface *yab)
{
	yab->RemoveItem(title,_L(item));
}

void yi_ClearItems(const char* title, YabInterface *yab)
{
	yab->ClearItems(title);
}

void yi_DrawText(double x, double y, const char* text, const char* window, YabInterface* yab)
{
	yab->DrawText(BPoint(x,y), _L(text), window);
}

void yi_DrawRect(double x1, double y1, double x2, double y2, const char* window, YabInterface* yab)
{
	yab->DrawRect(BRect(x1,y1,x2,y2),window);
}

void yi_DrawClear(const char* window, YabInterface* yab)
{
	yab->DrawClear(window, false);
}

void yi_CreateAlert(const char* text, const char* button1, const char* type, YabInterface* yab)
{
	yab->CreateAlert(_L(text),_L(button1),type);
}

void yi_CreateText(double x, double y, const char* id, const char* text, const char* window, YabInterface* yab)
{
	yab->CreateText(x,y,id,_L(text),window);
}

void yi_Text2(double x1, double y1, double x2, double y2, const char* id, const char* text, const char* window, YabInterface* yab)
{
	yab->Text2(BRect(x1,y1,x2,y2),id,_L(text),window);
}

void yi_TextAlign(const char* txt, const char *option, YabInterface *yab)
{
	yab->TextAlign(txt, option);
}

void yi_Translate(char* text, char result[])
{
	#ifdef ZETA
	result[0] = '\0';
	// char* tmp = strdup(text);
	const char* token;
	const char delimiters[] = ":";

	token = strtok(text, delimiters);
	while(token!=NULL)
	{
		strcat(result,_T(token));
		token = strtok(NULL, delimiters);
		if(token!=NULL) strcat(result,":");
	}

/*
	BString tmp(text);
	BString ret("");
	int j = tmp.FindFirst(':');
	if(j==B_ERROR)
		return _T(text);
	int i = 0;
	while(j!=B_ERROR)
	{
		BString t;
		tmp.CopyInto(t,i,j-i);
		i = j+1;
		j = tmp.FindFirst(':',i);
		ret += _T(t.String());
		ret += ":";
		if(j==B_ERROR)
		{
			tmp.CopyInto(t,i,tmp.Length()-i);
			ret += _T(t.String());
		}
	}
	return ret.String();
*/
	#else
		strcpy(result,text);
		// return text;
	#endif
}

void yi_SetLocalize()
{
	localize = true;
}

void yi_SetLocalize2(const char* path, YabInterface *yab)
{
	localize = true;
	yab->SetLocalize(path);
}

const char* yi_LoadFilePanel(const char* mode, const char* title, const char* directory, YabInterface *yab)
{
	return yab->FilePanel(mode, _L(title), directory, "");
}

const char* yi_SaveFilePanel(const char* mode, const char* title, const char* directory, const char* filename, YabInterface *yab)
{
	return yab->FilePanel(mode, _L(title), directory, filename);
}

void yi_SetLayout(const char* layout, const char* window, YabInterface *yab)
{
	yab->SetLayout(layout, window);
}

void yi_WindowSet1(const char* option, const char* value, const char* window, YabInterface *yab)
{
	yab->WindowSet(option, value, window);
}

void yi_WindowSet2(const char* option, int r, int g, int b, const char* window, YabInterface *yab)
{
	yab->WindowSet(option, r, g, b, window);
}

void yi_WindowSet3(const char* option, double x, double y, const char* window, YabInterface *yab)
{
	yab->WindowSet(option,x,y, window);
}

void yi_WindowSet4(const char* option, const char* window, YabInterface *yab)
{
	yab->WindowSet(option, window);
}

void yi_WindowClear(const char* window, YabInterface *yab)
{
	yab->WindowClear(window);
}

void yi_TextEdit(double x1, double y1, double x2, double y2, const char* title, int scrollbar, const char* window, YabInterface *yab)
{
	yab->TextEdit(BRect(x1,y1,x2,y2), title, scrollbar, window);
}

void yi_TextAdd(const char* title, const char* text, YabInterface *yab)
{
	yab->TextAdd(title,text);
}

void yi_TextSet(const char* title, const char* option, YabInterface *yab)
{
	yab->TextSet(title,option);
}

void yi_TextSet2(const char* title, const char* option, int value, YabInterface *yab)
{
	yab->TextSet(title,option,value);
}

void yi_TextSet3(const char* title, const char* option, const char* value, YabInterface *yab)
{
	yab->TextSet(title,option,value);
}

void yi_TextColor1(const char* title, const char* option, const char* command, YabInterface *yab)
{
	yab->TextColor(title,option,command);
}

void yi_TextColor2(const char* title, const char* option, int r, int g, int b, YabInterface *yab)
{
	yab->TextColor(title,option,r,g,b);
}

int yi_TextGet2(const char* title, const char* option, YabInterface *yab)
{
	return yab->TextGet(title,option);
}

const char* yi_TextGet3(const char* title, int linenum, YabInterface *yab)
{
	return yab->TextGet(title,linenum);
}

double yi_TextGet4(const char* title, const char* option, int linenum, YabInterface *yab)
{
	return yab->TextGet(title,option,linenum);
}

int yi_TextGet5(const char* title, const char* option, const char* option2, YabInterface *yab)
{
	return yab->TextGet(title,option,option2);
}

void yi_TextClear(const char* title, YabInterface *yab)
{
	yab->TextClear(title);
}

const char* yi_TextGet(const char* title, YabInterface *yab)
{
	return yab->TextGet(title);
}

void yi_DrawSet1(const char* option, const char* window, YabInterface *yab)
{
	return yab->DrawSet1(option, window);
}

void yi_DrawSet2(int fillorstroke, const char* mypattern, YabInterface *yab)
{
	return yab->DrawSet2(fillorstroke, mypattern);
}

void yi_View(double x1, double y1, double x2, double y2, const char* id, const char* view, YabInterface *yab)
{
	yab->View(BRect(x1,y1,x2,y2), id, view);
}

void yi_BoxView(double x1, double y1, double x2, double y2, const char* id, const char* text, int lineType, const char* view, YabInterface *yab)
{
	yab->BoxView(BRect(x1,y1,x2,y2), id, _L(text), lineType, view);
}

void yi_Tab(double x1, double y1, double x2, double y2, const char* id, const char* names, const char* view, YabInterface *yab)
{
	yab->Tab(BRect(x1,y1,x2,y2), id, names, view);
}

void yi_TabSet(const char* id, int num, YabInterface *yab)
{
	yab->TabSet(id, num);
}

void yi_TabAdd(const char* id, const char* tabname, YabInterface *yab)
{
	yab->TabAdd(id, _L(tabname));
}

void yi_TabDel(const char* id, int num, YabInterface *yab)
{
	yab->TabDel(id, num);
}

int yi_TabViewGet(const char* id, YabInterface *yab) 
{
	return yab->TabViewGet(id);
} 

void yi_DrawDot(double x, double y, const char* window, YabInterface *yab)
{
	yab->DrawDot(x,y, window);
}

void yi_DrawLine(double x1, double y1, double x2, double y2, const char* window, YabInterface *yab)
{
	yab->DrawLine(x1,y1,x2,y2, window);
}

void yi_DrawCircle(double x, double y, double r, const char* window, YabInterface *yab)
{
	yab->DrawCircle(x,y,r, window);
}

void yi_DrawEllipse(double x, double y, double r1, double r2, const char* window, YabInterface *yab)
{
	yab->DrawEllipse(x,y,r1,r2, window);
}

void yi_DrawCurve(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, const char* window, YabInterface *yab)
{
	yab->DrawCurve(x1,y1,x2,y2,x3,y3,x4,y4, window);
}

void yi_Slider1(double x1, double y1, double x2, double y2, const char* id, const char* title, int min, int max, const char* view, YabInterface *yab)
{
	yab->Slider(BRect(x1,y1,x2,y2), id, _L(title), min, max, view);
}

void yi_Slider2(double x1, double y1, double x2, double y2, const char* id, const char* title, int min, int max, const char* option, const char* view, YabInterface *yab)
{
	yab->Slider(BRect(x1,y1,x2,y2), id, _L(title), min, max, option, view);
}

void yi_SetSlider1(const char* id, const char* label1, const char* label2, YabInterface *yab)
{
	yab->SetSlider(id, _L(label1), _L(label2));
}

void yi_SetSlider2(const char* id, const char* bottomtop, int count, YabInterface *yab)
{
	yab->SetSlider(id, bottomtop, count);
}

void yi_SetSlider3(const char* id, const char* part, int r, int g, int b, YabInterface *yab)
{
	yab->SetSlider(id, part, r,g,b);
}

void yi_SetSlider4(const char* id, int value, YabInterface *yab)
{
	yab->SetSlider(id, value);
}

void yi_SetOption1(const char* id, const char* option, const char* value, YabInterface *yab)
{
	yab->SetOption(id,option,value);
}

void yi_SetOption2(const char* id, const char* option, int r, int g, int b, YabInterface *yab)
{
	yab->SetOption(id,option,r,g,b);
}

void yi_SetOption3(const char* id, const char* option, double x, double y, YabInterface *yab)
{
	yab->SetOption(id,option,x,y);
}

void yi_SetOption4(const char* id, const char* option, YabInterface *yab)
{
	yab->SetOption(id,option);
}

void yi_SetOption5(const char* id, const char* option, int value, YabInterface *yab)
{
	yab->SetOption(id,option,value);
}

void yi_DropZone(const char* view, YabInterface *yab)
{
	yab->DropZone(view);
}

void yi_ColorControl1(double x, double y, const char* id, const char* view, YabInterface* yab)
{
	yab->ColorControl(x,y,id,view);
}

void yi_ColorControl2(const char* id, int r, int g, int b, YabInterface* yab)
{
	yab->ColorControl(id,r,g,b);
}

void yi_TextControl2(const char* id, const char* text, YabInterface* yab)
{
	yab->TextControl(id,_L(text));
}

void yi_TextControl3(const char* id, int mode, YabInterface* yab)
{
	yab->TextControl(id,mode);
}

void yi_TextControl4(const char* id, YabInterface* yab)
{
	yab->TextControl(id);
}

void yi_TreeBox1(double x1, double y1, double x2, double y2, const char* id, int scrollbarType, const char* view, YabInterface* yab)
{
	yab->TreeBox1(BRect(x1,y1,x2,y2), id, scrollbarType, view);
}

void yi_TreeBox2(const char* id, const char* item, YabInterface* yab)
{
	yab->TreeBox2(id,_L(item));
}

void yi_TreeBox3(const char* id, const char* head, const char* item, int isExpanded, YabInterface* yab)
{
	yab->TreeBox3(id,_L(head),_L(item),isExpanded);
}

void yi_TreeBox4(const char* id, YabInterface* yab)
{
	yab->TreeBox4(id);
}

void yi_TreeBox5(const char* id, const char* item, YabInterface* yab)
{
	yab->TreeBox5(id,_L(item));
}

void yi_TreeBox7(const char* id, int pos, YabInterface* yab)
{
	yab->TreeBox7(id,pos);
}

void yi_TreeBox8(const char* id, int pos, YabInterface* yab)
{
	yab->TreeBox8(id,pos);
}

void yi_TreeBox9(const char* id, const char* head, const char* item, YabInterface* yab)
{
	yab->TreeBox9(id,_L(head), _L(item));
}

void yi_TreeBox10(const char* id, const char* head, YabInterface* yab)
{
	yab->TreeBox10(id,_L(head));
}

void yi_TreeBox11(const char* id, const char* head, YabInterface* yab)
{
	yab->TreeBox11(id,_L(head));
}

void yi_TreeBox12(const char* id, const char* item, int pos, YabInterface* yab)
{
	yab->TreeBox12(id,_L(item), pos);
}

const char* yi_TreeboxGet(const char* treebox, int pos, YabInterface *yab)
{
	return yab->TreeboxGet(treebox, pos);
}

int yi_TreeboxCount(const char* treebox, YabInterface *yab)
{
	return yab->TreeboxCount(treebox);
}

void yi_ButtonImage(double x,double y,const char* id,const char* enabledon, const char* enabledoff, const char *disabled, const char* view, YabInterface *yab)
{
	yab->ButtonImage(x,y, id, enabledon, enabledoff, disabled, view);
}

void yi_CheckboxImage(double x,double y,const char* id,const char* enabledon, const char* enabledoff, const char *disabledon, const char *disabledoff, int isActivated, const char* view, YabInterface *yab)
{
	yab->CheckboxImage(x,y, id, enabledon, enabledoff, disabledon, disabledoff, isActivated, view);
}

void yi_CheckboxSet(const char* id, int isActivated, YabInterface* yab)
{
	yab->CheckboxSet(id, isActivated);
}

void yi_RadioSet(const char* id, int isActivated, YabInterface* yab)
{
	yab->RadioSet(id, isActivated);
}

void yi_ToolTip(const char* view, const char* text, YabInterface *yab)
{
	yab->ToolTips(view,_L(text));
}

void yi_ToolTipColor(const char* color, int r, int g, int b, YabInterface *yab)
{
	yab->ToolTipsColor(color,r,g,b);
}

void yi_TreeSort(const char* view, YabInterface *yab)
{
	yab->TreeSort(view);
}

void yi_ListSort(const char* view, YabInterface *yab)
{
	yab->ListSort(view);
}

void yi_FileBox(double x1, double y1, double x2, double y2, const char* id, int scrollbartype, const char *option, const char* view, YabInterface *yab)
{
	yab->FileBox(BRect(x1,y1,x2,y2), id, scrollbartype, option, view);
}

void yi_FileBoxAdd2(const char* columnbox, const char* name, int pos, double maxWidth, double minWidth, double width, const char* option, YabInterface *yab)
{
	yab->FileBoxAdd(columnbox, _L(name), pos, maxWidth, minWidth, width, option);
}

void yi_FileBoxClear(const char* view, YabInterface *yab)
{
	yab->FileBoxClear(view);
}

void yi_ColumnBoxAdd(const char* id, int column, int position, int height, const char* item, YabInterface *yab)
{
	yab->ColumnBoxAdd(id, column, position, height, _L(item));
}

void yi_ColumnBoxSelect(const char *columnbox, int position, YabInterface *yab)
{
	yab->ColumnBoxSelect(columnbox, position);
}

void yi_ColumnBoxRemove(const char *columnbox, int position, YabInterface *yab)
{
	yab->ColumnBoxRemove(columnbox, position);
}

void yi_ColumnBoxColor(const char *columnbox, const char* option, int r, int g, int b, YabInterface *yab)
{
	yab->ColumnBoxColor(columnbox, option, r,g,b);
}

const char* yi_ColumnBoxGet(const char *columnbox, int column, int position, YabInterface *yab)
{
	return yab->ColumnBoxGet(columnbox, column, position);
}

int yi_ColumnBoxCount(const char *columnbox, YabInterface *yab)
{
	return yab->ColumnBoxCount(columnbox);
}

const char* yi_TextControlGet(const char* id, YabInterface* yab)
{
	return yab->TextControlGet(id);
}

int yi_DeskbarPosition(YabInterface *yab)
{
	return yab->DeskbarParam("position");
}

int yi_DeskbarExpanded(YabInterface *yab)
{
	return yab->DeskbarParam("expanded");
}

int yi_DeskbarWidth(YabInterface *yab)
{
	return yab->DeskbarParam("width");
}

int yi_DeskbarHeight(YabInterface *yab)
{
	return yab->DeskbarParam("height");
}

int yi_DeskbarX(YabInterface *yab)
{
	return yab->DeskbarParam("x");
}

int yi_DeskbarY(YabInterface *yab)
{
	return yab->DeskbarParam("y");
}

int yi_DesktopWidth(YabInterface *yab)
{
	return yab->DesktopParam(true);
}

int yi_DesktopHeight(YabInterface *yab)
{
	return yab->DesktopParam(false);
}

int yi_WindowGet(const char* view, const char* option, YabInterface *yab)
{
	return yab->WindowGet(view,option);
}

int yi_ViewGet(const char* view, const char* option, YabInterface *yab) //vasper
{
	return yab->ViewGet(view,option);
}

void yi_ClipboardCopy(const char* text, YabInterface *yab)
{
	yab->ClipboardCopy(text);
}

int yi_Printer(const char* docname, const char *view, const char* config, YabInterface *yab)
{
	return yab->Printer(docname, view,config);
}

void yi_PrinterConfig(const char* config, YabInterface *yab)
{
	yab->PrinterConfig(config);
}

const char* yi_ClipboardPaste(YabInterface *yab)
{
	return yab->ClipboardPaste();
}

int yi_NewAlert(const char* text, const char* button1, const char* button2, const char* button3, const char* option, YabInterface *yab)
{
	return yab->NewAlert(_L(text), _L(button1), _L(button2), _L(button3), option);
}

void yi_Calendar1(double x, double y, const char* id, const char* format, const char* date, const char* view, YabInterface *yab)
{
	yab->Calendar(x,y, id, format, date, view);
}

const char* yi_Calendar2(const char* id, YabInterface *yab)
{
	return yab->Calendar(id);
}

void yi_Calendar3(const char* id, const char* date, YabInterface *yab)
{
	yab->Calendar(id, date);
}

const char* yi_ListboxGet(const char* listbox, int pos, YabInterface *yab)
{
	return yab->ListboxGet(listbox, pos);
}

int yi_ListboxCount(const char* listbox, YabInterface *yab)
{
	return yab->ListboxCount(listbox);
}

void yi_ListboxAdd1(const char* listbox, const char* item, YabInterface *yab)
{	
	yab->ListboxAdd(listbox,_L(item));
}

void yi_ListboxAdd2(const char* listbox, int pos,  const char* item, YabInterface *yab)
{	
	yab->ListboxAdd(listbox, pos, _L(item));
}

void yi_ListboxSelect(const char* listbox, int pos, YabInterface *yab)
{	
	yab->ListboxSelect(listbox,pos);
}

void yi_ListboxRemove(const char* listbox, int pos, YabInterface *yab)
{	
	yab->ListboxRemove(listbox,pos);
}

void yi_Scrollbar(const char* id, int format, const char* view, YabInterface *yab)
{
	yab->Scrollbar(id, format, view);
}

void yi_ScrollbarSet1(const char* scrollview, const char* option, double position, YabInterface *yab)
{
	yab->ScrollbarSet(scrollview, option, position);
}

void yi_ScrollbarSet2(const char* scrollview, const char* option, double opt1, double opt2, YabInterface *yab)
{
	yab->ScrollbarSet(scrollview, option, opt1, opt2);
}

double yi_ScrollbarGet(const char* scrollview, const char* option, YabInterface *yab)
{
	return yab->ScrollbarGet(scrollview, option);
}

void yi_SplitView1(double x1,double y1,double x2,double y2, const char* id, int isVertical, int style, const char* view, YabInterface *yab)
{
	yab->SplitView(BRect(x1,y1,x2,y2), id, isVertical, style, view);
}

void yi_SplitView2(const char* splitView, const char* option, double position, YabInterface *yab)
{
	yab->SplitView(splitView, option, position);
}

void yi_SplitView3(const char* splitView, const char* option, double left, double right, YabInterface *yab)
{
	yab->SplitView(splitView, option, left, right);
}

double yi_SplitViewGet(const char* splitView, const char* option, YabInterface *yab)
{
	return yab->SplitViewGet(splitView, option);
}

void yi_StackView1(double x1,double y1,double x2,double y2, const char* id, int number, const char* view, YabInterface *yab)
{
	yab->StackViews(BRect(x1,y1,x2,y2), id, number, view);
}

void yi_StackView2(const char* stackView, int num, YabInterface *yab)
{
	yab->StackViews(stackView, num);
}

int yi_StackViewGet(const char* stackView, YabInterface *yab)
{
	return yab->StackViewGet(stackView);
}

void yi_DrawSet3(const char* option, int transparency, YabInterface *yab)
{
	yab->DrawSet3(option, transparency);
}

extern void yi_TextURL1(double x, double y, const char* id, const char* text, const char* url, const char* view, YabInterface *yab)
{
	yab->TextURL(x,y, id, _L(text), url, view);
}

void yi_TextURL2(const char* id, const char* option, int r, int g, int b, YabInterface *yab)
{
	yab->TextURL(id, option, r,g,b);
}

void yi_Menu2(const char* menuHead, int isRadio, const char* view, YabInterface *yab)
{
	yab->Menu(_L(menuHead), isRadio, view);
}

void yi_SubMenu1(const char* menuHead, const char* menuItem, const char* subMenuItem, const char* modifiers, const char* view, YabInterface *yab)
{
	yab->SubMenu(_L(menuHead), _L(menuItem), _L(subMenuItem), modifiers, view);
}

void yi_SubMenu2(const char* menuHead, const char* menuItem, int isRadio, const char* view, YabInterface *yab)
{
	yab->SubMenu(_L(menuHead), _L(menuItem), isRadio, view);
}

void yi_SpinControl1(double x, double y, const char* id, const char* label, int min, int max, int step, const char* view, YabInterface *yab)
{
	yab->SpinControl(x,y, id, _L(label), min, max, step, view);
}

void yi_SpinControl2(const char* spinControl, int value, YabInterface *yab)
{
	yab->SpinControl(spinControl, value);
}

int yi_SpinControlGet(const char *spinControl, YabInterface *yab)
{
	return yab->SpinControlGet(spinControl);
}

const char* yi_PopUpMenu(double x, double y, const char* menuItems, const char* view, YabInterface *yab)
{
	return yab->PopUpMenu(x,y,menuItems,view);
}

void yi_DropBoxSelect(const char* dropbox, int position, YabInterface *yab)
{
	yab->DropBoxSelect(dropbox, position);
}

void yi_DropBoxClear(const char* dropbox, YabInterface *yab)
{
	yab->DropBoxClear(dropbox);
}

void yi_DropBoxRemove(const char* dropbox, int position, YabInterface *yab)
{
	yab->DropBoxRemove(dropbox,position);
}

int yi_DropBoxCount(const char* dropbox, YabInterface *yab)
{
	return yab->DropBoxCount(dropbox);
}

const char* yi_DropBoxGet(const char* dropbox, int position, YabInterface *yab)
{
	return yab->DropBoxGet(dropbox, position);
}

int yi_ColorControlGet(const char* colorcontrol, const char* option, YabInterface *yab)
{
	return yab->ColorControlGet(colorcontrol, option);
}

int yi_SliderGet(const char* slider, YabInterface *yab)
{
	return yab->SliderGet(slider);
}

double yi_DrawGet1(const char* option, const char* txt, const char* view, YabInterface *yab)
{
	return yab->DrawGet(option, txt, view);
}

double yi_DrawGet2(const char* option, const char* view, YabInterface *yab)
{
	return yab->DrawGet(option, "", view);
}

const char* yi_DrawGet3(const char* option, YabInterface *yab)
{
	return yab->DrawGet(option);
}

void yi_SubMenu3(const char* menuHead, const char* menuItem, const char* subMenuItem, const char* option, const char* view, YabInterface *yab)
{
	yab->SubMenu3(_L(menuHead), _L(menuItem), _L(subMenuItem), option, view);
}

void yi_Menu3(const char* menuHead, const char* menuItem, const char* option,const char* view, YabInterface *yab)
{
	yab->Menu3(_L(menuHead), _L(menuItem), option, view);
}

double yi_MenuHeight(YabInterface *yab)
{
	return yab->MenuHeight();
}

double yi_TabHeight(YabInterface *yab)
{
	return yab->TabHeight();
}

double yi_ScrollbarWidth(YabInterface *yab)
{
	return yab->ScrollbarWidth();
}

void yi_exit(int code, YabInterface *yab)
{
	yab->KillThread(code);
}

const int yi_IsMouseIn(const char* view, YabInterface *yab)
{
	return yab->IsMouseIn(view);
}
	
const char* yi_GetMouseIn(YabInterface *yab)
{
	return yab->GetMouseIn();
}
		
	
const char* yi_KeyboardMessages(const char* view, YabInterface* yab)
{
	return yab->KeyboardMessages(view);
}

const char* yi_GetMouseMessages(const char* view, YabInterface* yab)
{
	return yab->GetMouseMessages(view);
}

const char* yi_CheckMessages(YabInterface* yab)
{
	return yab->GetMessageString();
}

int yi_MessageSend(const char* app, const char* msg,YabInterface* yab)
{
	return yab->MessageSend(app,msg);
}

int yi_ThreadKill(const char* option, int id,YabInterface* yab)
{
	return yab->ThreadKill(option, id);
}

int yi_ThreadGet(const char* option, const char* appname,YabInterface* yab)
{
	return yab->ThreadGet(option, appname);
}

void yi_SetCurrentLineNumber(int line, const char* libname, YabInterface* yab)
{
	yab->SetCurrentLineNumber(line, libname);
}

void yi_SetMainFileName(const char* name, YabInterface* yab)
{
	yab->SetMainFileName(name);
}

void yi_beep()
{
	// beep();
}

void yi_Bitmap(double w, double h, const char* id,YabInterface* yab)
{
	yab->Bitmap(w,h,id);
}

void yi_BitmapDraw(double x, double y, const char* bitmap, const char* mode, const char* view,YabInterface* yab)
{
	yab->BitmapDraw(x,y, bitmap, mode, view);
}

void yi_BitmapDraw2(double x1, double y1, double x2, double y2, const char* bitmap, const char* mode, const char* view,YabInterface* yab)
{
	yab->BitmapDraw(BRect(x1,y1,x2,y2), bitmap, mode, view);
}

void yi_BitmapGet(double x1, double y1, double x2, double y2, const char* id, const char* bitmap, YabInterface* yab)
{
	yab->BitmapGet(BRect(x1,y1,x2,y2), id, bitmap);
}

void yi_BitmapGet2(double w, const char* id, const char* path, YabInterface* yab)
{
	yab->BitmapGet(w, id, path);
}

void yi_BitmapGetIcon(const char* id, const char* option, const char* path, YabInterface* yab)
{
	yab->BitmapGetIcon(id, option, path);
}

void yi_BitmapDrag(const char* bitmap,YabInterface* yab)
{
	yab->BitmapDrag(bitmap);
}

void yi_BitmapRemove(const char* bitmap,YabInterface* yab)
{
	yab->BitmapRemove(bitmap);
}

void yi_Screenshot(double x1, double y1, double x2, double y2, const char* bitmap, YabInterface *yab)
{
	return yab->Screenshot(BRect(x1,y1,x2,y2), bitmap);
}

int yi_BitmapSave(const char* id, const char* filename, const char* type, YabInterface* yab)
{
	return yab->BitmapSave(id, filename, type); //, type);
}

void yi_Canvas(double x1, double y1, double x2, double y2, const char* id, const char* view, YabInterface *yab)
{
	yab->Canvas(BRect(x1,y1,x2,y2), id,view);
}

int yi_Sound(const char* filename, YabInterface* yab)
{
	return yab->Sound(filename);
}

void yi_SoundStop(int id, YabInterface* yab)
{
	yab->SoundStop(id);
}

void yi_SoundWait(int id, YabInterface* yab)
{
	yab->SoundWait(id);
}

void yi_ShortCut(const char* view, const char* key, const char* msg, YabInterface *yab)
{
	yab->ShortCut(view,key,msg);
}

int yi_IsComputerOn(YabInterface *yab)
{
	return yab->IsComputerOn();
}

void yi_DrawSet4(const char* option, const char* color,const char* view, YabInterface* yab)
{
	yab->DrawSet(option, color, view);
}

void yi_Treebox13(const char* id,const char* option, int pos, YabInterface* yab)
{
	yab->Treebox13(id, option, pos);
}

int yi_TreeboxGetOpt(const char* id, const char* option, int pos, YabInterface* yab)
{
	return yab->TreeboxGetOpt(id, option, pos);
}

int yi_ListboxGetNum(const char* id, YabInterface* yab)
{
	return yab->ListboxGetNum(id);
}

int yi_DropboxGetNum(const char* id, YabInterface* yab)
{
	return yab->DropboxGetNum(id);
}

int yi_TreeboxGetNum(const char* id, YabInterface* yab)
{
	return yab->TreeboxGetNum(id);
}

int yi_ColumnboxGetNum(const char* id, YabInterface* yab)
{
	return yab->ColumnboxGetNum(id);
}

int yi_DrawGet4(double x, double y, const char* option, const char* view, YabInterface* yab)
{
	return yab->DrawGet(BPoint(x,y),option,view);
}

void yi_MouseSet(const char* opt, YabInterface *yab)
{	
	yab->MouseSet(opt);
}

void yi_StatusBar(double x1, double y1, double x2, double y2, const char* id, const char* label1, const char* label2, const char* view, YabInterface *yab)
{
	yab->StatusBar(BRect(x1, y1, x2, y2), id, label1, label2, view);
}

void yi_StatusBarSet(const char* id, const char* label1, const char* label2, double state, YabInterface *yab)
{
	yab->StatusBarSet(id, label1, label2, state);
}

