//CC_Infos.h

#include <FL/Fl_Group.H> 

#ifndef CC_INFOS_H
#define CC_INFOS_H

class CC_Infos {
	public:
		int posx_, posy_;
		Fl_Box *mb_;
		Fl_Box *yb_;
		int d,m,y;
		Fl_Window *win_;
		Fl_Window *oldwin[10000];
		Fl_Input *input_;
		int winx_,winy_;
		int num;		
};
#endif
