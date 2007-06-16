//
// sortapp -- Test the Fl_Table_Row class with an actual app
//
//     Click on the headers to sort that column.
//

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Table_Row.H>

#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>		// STL sort

#ifdef MICROSOFT
#define popen _popen
#endif /*MICROSOFT*/

// A row of columns
class Row
{
public:
    std::vector<char*> words;
};

// Sort class to handle sorting strings by column
class SortColumn
{
    int _col, _reverse;
public:
    SortColumn(int col, int reverse)
    {
	_col = col;
	_reverse = reverse;
    }
    bool operator()(const Row &a, const Row &b)
    {
        const char *ap = ( _col < (int)a.words.size() ) ? a.words[_col] : "",
	           *bp = ( _col < (int)b.words.size() ) ? b.words[_col] : "";
	if ( _reverse ) { return(strcmp(ap, bp) > 0); }
	else            { return(strcmp(ap, bp) < 0); }
    }
};

//
// Sample application class of an Fl_Table_Row
//
class MyTable : public Fl_Table_Row
{
private:
    std::vector<Row> _rowdata;	// data in each row
    int _maxcols;		// max # columns in all rows
    int _sort_reverse,
        _sort_lastcol;

    static void event_callback(Fl_Widget*, void*);
    void event_callback2();					// callback for table events

protected:
    void draw_cell(TableContext context, int R=0, int C=0, 	// table cell drawing
    		   int X=0, int Y=0, int W=0, int H=0);
    void sort_column(int col, int reverse=0);			// sort table by a column

public:
    MyTable(int x, int y, int w, int h, const char *l=0) : Fl_Table_Row(x,y,w,h,l)
    {
        _maxcols = 0;
        _sort_reverse = 0;
        _sort_lastcol = -1;
	end();
        callback(event_callback, (void*)this);
    }
    ~MyTable() { }

    void load_command(const char *cmd);		// load the output of a command into table
    void autowidth(int pad);			// automatically set column widths to data
};

// Sort a column up or down
void MyTable::sort_column(int col, int reverse)
{
    std::sort(_rowdata.begin(), _rowdata.end(), SortColumn(col, reverse));
    redraw();
}

// Handle drawing all cells in table
void MyTable::draw_cell(TableContext context, 
			int R, int C, int X, int Y, int W, int H)
{
    char *s = "";

    if ( R < (int)_rowdata.size() && C < (int)_rowdata[R].words.size() )
	s = _rowdata[R].words[C];

    switch ( context )
    {
	case CONTEXT_STARTPAGE:
	    fl_font(FL_COURIER, 16);
	    return;

	case CONTEXT_ROW_HEADER:
	    fl_color(FL_RED);
	    fl_rectf(X, Y, W, H);
	    return;

	case CONTEXT_COL_HEADER:
	    fl_push_clip(X, Y, W, H);
	    {
		static char *head[] = 
#ifdef MICROSOFT
		    { "Date", "Time", "Size", "Filename", "", "", "", "", "" };
#else /*MICROSOFT*/
		    { "Perms", "#L", "Own", "Group", "Size", "Date", "", "", "Filename" };
#endif /*MICROSOFT*/

		fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, color());
		if ( C < 9 )
		{
		    fl_color(FL_BLACK);
		    fl_draw(head[C], X+2, Y, W, H, FL_ALIGN_LEFT, 0, 0);	// +2=pad left

		    // DRAW SORT ARROW
		    if ( C == _sort_lastcol )
		    {
			int xlft = X+(W-6)-8,
			    xctr = X+(W-6)-4,
			    xrit = X+(W-6)-0,
			    ytop = Y+(H/2)-4,
			    ybot = Y+(H/2)+4;

			if ( _sort_reverse )
			{
			    // ENGRAVED DOWN ARROW
			    fl_color(FL_WHITE);
			    fl_line(xrit, ytop, xctr, ybot);
			    fl_color(41);	// dark gray
			    fl_line(xlft, ytop, xrit, ytop);
			    fl_line(xlft, ytop, xctr, ybot);
			}
			else
			{
			    // ENGRAVED UP ARROW
			    fl_color(FL_WHITE);
			    fl_line(xrit, ybot, xctr, ytop);
			    fl_line(xrit, ybot, xlft, ybot);
			    fl_color(41);	// dark gray
			    fl_line(xlft, ybot, xctr, ytop);
			}
		    }
		}
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

		fl_color(FL_BLACK);
		fl_draw(s, X+2, Y, W, H, FL_ALIGN_LEFT);	// +2=pad left

		// BORDER
		fl_color(FL_LIGHT2); 
		fl_rect(X, Y, W, H);
	    }
	    fl_pop_clip();
	    return;
	}

	case CONTEXT_ENDPAGE:
	case CONTEXT_RC_RESIZE:
	case CONTEXT_NONE:
	case CONTEXT_TABLE:
	    return;
    }
}

// Automatically set column widths to widest data in each column
void MyTable::autowidth(int pad)
{
    fl_font(FL_COURIER, 16);

    // Initialize all column widths to lowest value
    for ( int c=0; c<cols(); c++ )
	{ col_width(c, pad); }

    for ( int r=0; r<(int)_rowdata.size(); r++ )
    {
	int w, h;
	for ( int c=0; c<(int)_rowdata[r].words.size(); c++ )
	{
	    fl_measure(_rowdata[r].words[c], w, h, 0);
	    if ( (w + pad) > col_width(c))
		{ col_width(c, w + pad); }
	}
    }
    table_resized();
    redraw();
}

// Load table with output of 'cmd'
void MyTable::load_command(const char *cmd)
{
    char s[512];
    FILE *fp = popen(cmd, "r");

    _maxcols = 0;

    for ( int r=0; fgets(s, sizeof(s)-1, fp); r++ )
    {
	if ( strlen(s) > 1 )
	    s[strlen(s)-1] = 0;

	// new row
	Row newrow;
	_rowdata.push_back(newrow);

	char *ss = s, *word = s;
	while ( *ss )
	{
	    // Found white space? 
	    if ( strchr(" \t", *ss) )
	    {
		// Make previous data a word, add to column
		{
		    char tmp = *ss;
		    *ss = 0;
		    _rowdata[r].words.push_back(strdup(word));
		    *ss = tmp;
		}

		// Skip over all white
		while ( strchr(" \t", *ss) )
		    { ++ss; }

		// Next word starts at first non-white
		word = ss;
	    }
	    ss++;
	}

	// last word
	_rowdata[r].words.push_back(strdup(word));

	// keep track of most # columns
	if ( (int)_rowdata[r].words.size() > _maxcols )
	    { _maxcols = (int)_rowdata[r].words.size(); }
    }

    // Initialize table rows/cols based on data loaded
    rows((int)_rowdata.size());
    cols(_maxcols);

    // Auto-calculate widths, with 20 pixel padding
    autowidth(20);
}

// Callback whenever someone clicks on different parts of the table
void MyTable::event_callback(Fl_Widget*, void *data)
{
    MyTable *o = (MyTable*)data;
    o->event_callback2();
}

void MyTable::event_callback2()
{
    int // R = callback_row(),			// currently unused
        C = callback_col();
    TableContext context = callback_context();
    switch ( context )
    {
	case CONTEXT_COL_HEADER:		// someone clicked on column header
	{
	    if ( Fl::event() == FL_RELEASE && Fl::event_button() == 1 )
	    {
		if ( _sort_lastcol == C )
		    { _sort_reverse ^= 1; }	// Click same column? Toggle sort
		else
		    { _sort_reverse = 0; } 	// Click diff column? Up sort 

		sort_column(C, _sort_reverse);
		_sort_lastcol = C;
	    }
	    break;
	}

	default:
	    return;
    }
}

int main()
{
    Fl_Window win(900,500);
    MyTable table(20, 20, win.w()-40, win.h()-40);
    table.selection_color(FL_YELLOW);
    table.col_header(1);
    table.col_resize(1);
    table.when(FL_WHEN_RELEASE);		// handle table events on release
#if defined(_WIN32)
    table.load_command("dir");			// load table with a directory listing
#else
    table.load_command("ls -la /var/tmp");	// load table with a directory listing
    // table.load_command("cat README");	// load table with a directory listing
#endif
    table.row_height_all(18);			// height of all rows
    win.end();
    win.resizable(table);
    win.show();
    return(Fl::run());
}

