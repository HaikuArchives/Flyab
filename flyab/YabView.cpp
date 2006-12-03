#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Tabs.H>
#include <iostream>
#include "YabView.h"
#include "global.h"
	
YabView::YabView(int x, int y, int width, int height, const char* id)
	: Fl_Group(x,y,width,height), YabWidget(id)
{
	highcolor = fl_rgb_color(0,0,0);
	lowcolor = fl_rgb_color(B_GREY);
	bgcolor = fl_rgb_color(B_GREY);
	hasmenu = false;
	highSolid = true;

	// remember the absolute coordinates
	fx = x;
	if(dynamic_cast<Fl_Tabs*>(parent()))
		fy = fy+2;
	else
		fy = y;
}

YabView::~YabView()
{
	for(int i=0; i<drawList.size(); i++)
	{
		YabDrawing *last = drawList[i];
		drawList[i] = NULL;
		if(last->command == 0) delete [] last->chardata;
		delete last;
	}
	drawList.clear();
}

void YabView::SetColor(int r, int g, int b)
{
	bgcolor = fl_rgb_color(r,g,b);
}

void YabView::AddDrawing(YabDrawing* drawing)
{
	drawList.push_back(drawing);
}

void YabView::FlushDrawings()
{
	for(int i=0; i<drawList.size(); i++)
	{
		YabDrawing *last = drawList[i];
		drawList[i] = NULL;
		if(last->command == 0) delete [] last->chardata;
		delete last;
	}
	drawList.clear();
	// this might be a workaround as well, but with less lines ;P
	hide();
	show();
}

void YabView::draw()
{
	fl_draw_box(FL_FLAT_BOX, fx,fy, w(), h(), bgcolor);
	fl_color(0,0,0);
	for(int i=0; i<drawList.size(); i++)
	{
		YabDrawing *e = drawList[i];
		switch(e->command)
		{
				case 0: // draw text
					fl_draw(e->chardata, e->x1, e->y1);
					break;
				case 1: // draw line
					fl_line(e->x1, e->y1, e->x2, e->y2); 
					break;
				case 2: // stroke ellipse
					fl_push_matrix();
					fl_translate((float)e->x1,(float)e->y1);
					fl_scale((float)e->x2,(float)e->y2);
					fl_begin_line();
					fl_arc(0,0, 1, 0, 360);
					fl_end_line();
					fl_pop_matrix();
					break;
				case 3: // fill ellipse
					fl_push_matrix();
					fl_translate((float)e->x1,(float)e->y1);
					fl_scale((float)e->x2,(float)e->y2);
					fl_begin_complex_polygon();
					fl_begin_loop();
					fl_arc(0,0, 1, 0, 360);
					fl_end_loop();
					fl_end_complex_polygon();
					fl_pop_matrix();
					break;
				case 4: // stroke rect
					fl_rect(e->x1, e->y1, e->x2-e->x1, e->y2-e->y1);
					break;
				case 5: // fill rect
					fl_draw_box(FL_FLAT_BOX, e->x1,e->y1, e->x2-e->x1,e->y2-e->y1, highcolor);
					break;
				case 6: // set highcolor
					highcolor = fl_rgb_color(e->r, e->g, e->b);
					if(highSolid) fl_color(highcolor);
					break;
				case 7: // set lowcolor
					lowcolor = fl_rgb_color(e->r, e->g, e->b);
					if(!highSolid) fl_color(lowcolor);
					break;
				case 8: // stroke bezier
					fl_begin_line();
					fl_curve(e->x1,e->y1,e->x2,e->y2,e->x3,e->y3,e->x4,e->y4);
					fl_end_line();
					break;
				case 9: // fill bezier
					fl_begin_complex_polygon();
					fl_begin_loop();
					fl_curve(e->x1,e->y1,e->x2,e->y2,e->x3,e->y3,e->x4,e->y4);
					fl_end_loop();
					fl_end_complex_polygon();
					break;
				case 10: // draw bitmap at x,y
					break;
				case 11: // draw bitmap at x1,y1 to x2,y2
					break;
				case 12: // set font
					fl_font(e->x1, e->y1);
					break;
				default:
					break;
		}
	}

	for (int i=0; i<children(); i++)
	{
		child(i)->redraw();
	}

	Fl_Group::draw();
}

bool YabView::HasMenu()
{
	return hasmenu;
}

void YabView::HasMenu(bool m)
{
	hasmenu = m;
}

