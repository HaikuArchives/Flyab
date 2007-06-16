//
// testtablerow-utf8 -- Simple test of Fl_Table_Row class with utf8 mods
//
#if defined(USEUTF8) && ( defined(MICROSOFT) || defined(LINUX) )
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/fl_utf8.H>		// UTF8
#include <FL/Fl_Table_Row.H>
#include <stdio.h>
#include <string.h>

//
// Simple demonstration class to derive from Fl_Table_Row
//
class DemoTable : public Fl_Table_Row
{
protected:
    void draw_cell(TableContext context,  		// table cell drawing
    		   int R=0, int C=0, int X=0, int Y=0, int W=0, int H=0);
//    void callback(TableContext context, 		// callback for table events
//    		   int R, int C);

public:
    DemoTable(int x, int y, int w, int h, const char *l=0) : Fl_Table_Row(x,y,w,h,l)
	{ end(); }
    ~DemoTable() { }
};

// Handle drawing all cells in table
void DemoTable::draw_cell(TableContext context, 
			  int R, int C, int X, int Y, int W, int H)
{
    static char s[40];
    sprintf(s, "%d/%d", R, C);		// text for each cell

    switch ( context )
    {
	case CONTEXT_STARTPAGE:
	    fl_font(FL_FREE_FONT, 16);
	    // fl_font(FL_HELVETICA, 16);
	    return;

	case CONTEXT_ROW_HEADER:
	case CONTEXT_COL_HEADER:
	    fl_push_clip(X, Y, W, H);
	    {
		fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, color());
		fl_color(FL_BLACK);
		fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
	    }
	    fl_pop_clip();
	    return;

	case CONTEXT_CELL:
	{
	    fl_push_clip(X, Y, W, H);
	    {
	        // BG COLOR
		fl_color( row_selected(R) ? selection_color() : FL_WHITE);
		fl_rectf(X, Y, W, H);

		// TEXT
		if ( R==5 )		// UTF8
		{
		    char *utfstr = "\xe4\xbd\x95\xe3\x82\x82\xe8\xa1"
		                   "\x8c\xe3\x82\x8b\xe3\x80\x82";
		    fl_color(FL_BLACK);
		    fl_draw(utfstr, X, Y, W, H, FL_ALIGN_CENTER);
		}
		else
		{
		    fl_color(FL_BLACK);
		    fl_draw(s, X, Y, W, H, FL_ALIGN_CENTER);
		}

		// BORDER
		fl_color(FL_LIGHT2); 
		fl_rect(X, Y, W, H);

		if ( type() != SELECT_NONE && 
		     Fl::focus() == this && 
		     is_selected(R,C) )
		{
		    draw_focus(FL_NO_BOX, X, Y, W, H);
	        }
	    }
	    fl_pop_clip();
	    return;
	}

	default:
	    return;
    }
}

void table_cb(Fl_Widget* o, void* data)
{
    Fl_Table *table = (Fl_Table*)data;
    fprintf(stderr, "%s callback: row=%d col=%d, context=%d, event=%d clicks=%d\n",
	(const char*)table->label(),
	(int)table->callback_row(),
	(int)table->callback_col(),
	(int)table->callback_context(),
	(int)Fl::event(),
	(int)Fl::event_clicks());
}

int main()
{
    int w = 900, h = 700;
    int t1x = 20, t1y = 20,         t1w = w - 40, t1h = ( (h-60) / 2 );
    int t2x = 20, t2y = t1y+t1h+20, t2w = w - 40, t2h = t1h;

  /* setup an extra font */
    Fl::set_font((Fl_Font)(FL_FREE_FONT),
	"-*-courier-medium-r-normal--*,"
	"-*-fixed-medium-*-*--*-*-*-*-*-*-iso8859-15,"
	"-*-symbol-*-*-*--*-*-*-*-*-*-adobe-fontspecific,"
	"-*-*dingbats-*-*-*--*-*-*-*-*-*-adobe-fontspecific,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-2,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-3,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-4,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-5,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-6,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-7,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-8,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-9,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-13,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-14,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-koi8-*,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-jisx0208.1983-*,"
	"-*-fixed-*-*-*--*-*-*-*-*-*-jisx0201.1976-*,"
	"-*-*-*-*-*--*-*-*-*-*-*-ksc5601.1987-*,"
	"-*-fixed-medium-r-normal--*-*-*-*-*-*-big5*-*,"
	"-*-unifont-*-*-*--*-*-*-*-*-*-iso10646-1");

    Fl_Window win(w, h);
    DemoTable table1(t1x, t1y, t1w, t1h, "Table 1");
    table1.selection_color(FL_YELLOW);
    table1.when(FL_WHEN_RELEASE);	// handle table events on release
    table1.rows(1001);
    table1.cols(31);
    table1.col_header(1);		// enable col header
    table1.col_resize(4);		// enable col resizing
    table1.row_header(1);		// enable row header
    table1.row_resize(4);		// enable row resizing
    table1.callback(table_cb, (void*)&table1);
    table1.when(FL_WHEN_CHANGED|FL_WHEN_RELEASE);
    table1.end();

    DemoTable table2(t2x, t2y, t2w, t2h, "Table 2");
    table2.selection_color(FL_YELLOW);
    table2.when(FL_WHEN_RELEASE);	// handle table events on release
    table2.rows(1001);
    table2.cols(31);
    table2.col_header(1);		// enable col header
    table2.col_resize(4);		// enable col resizing
    table2.row_header(1);		// enable row header
    table2.row_resize(4);		// enable row resizing
    table2.end();

    win.resizable(table1);
    win.show();
    return(Fl::run());
}
#else /*MICROSOFT|LINUX*/
#include <stdio.h>
int main()
{

#if ! defined(USEUTF8) && ( defined(MICROSOFT) || defined(LINUX) )
    fprintf(stderr, "Uncomment 'USEUTF8=' line in the Makefile.<YOUR_OS> "
                    "and rebuild all.\n");
#else /*UTF8*/
    fprintf(stderr, "UTF8 only implemented on Windows and Linux\n");
#endif /*UTF8*/

    return(0);
}
#endif /*MICROSOFT|LINUX*/

