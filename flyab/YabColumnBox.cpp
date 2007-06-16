#include "YabColumnBox.h"

void YabColumnBox::AddColumn(const char* item, int pos, Fl_Align align)
{
	if(pos<1) pos = 1;
	if(pos>columnCount+1) pos = columnCount+1;

	columnCount++;
	std::string t = item;
	myrows[0].resize(columnCount);
	alignments.resize(columnCount);

	for(int i=columnCount-1; i>=pos; i--)
	{
		myrows[0][i] = myrows[0][i-1];
		alignments[i] = alignments[i-1];
	}

	myrows[0][pos-1] = item;
	alignments[pos-1] = align;
}

//void YabColumnBox::AddItem(const char* item, int rowpos, int colpos)
void YabColumnBox::AddItem(const char* item, int colpos, int rowpos) 
{
	if(columnCount == 0 || colpos<1 || colpos>columnCount)
		return;

	if(rowpos>=myrows.size()) 
	{
		rowpos = myrows.size();
		std::vector<std::string> s;
		s.resize(columnCount);
		s[colpos-1] = item;
		myrows.push_back(s);
		rows(myrows.size()-1);
	}
	else
	{
		myrows[rowpos][colpos-1] = item;	
	}
}

// Handle drawing all cells in table
void YabColumnBox::draw_cell(TableContext context, int R, int C, int X, int Y, int W, int H)
{
	switch ( context )
	{
		case CONTEXT_STARTPAGE:
    			fl_font(FL_HELVETICA, B_FONT_SIZE);
    			return;
		case CONTEXT_ROW_HEADER:
		case CONTEXT_COL_HEADER:
    			fl_push_clip(X, Y, W, H);
    			{
				fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, color());
				fl_color(FL_BLACK);
				fl_draw(myrows[R][C].c_str(), X, Y, W, H, FL_ALIGN_CENTER);
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
				fl_color(row_selected(R) ? FL_WHITE : FL_BLACK);
				fl_draw(myrows[R+1][C].c_str(), X, Y, W, H, alignments[C]);
				// BORDER
				fl_color(FL_LIGHT2); 
				fl_rect(X, Y, W, H);
    			}
			fl_pop_clip();
			return;
		}

		default:
			return;
	}
}

