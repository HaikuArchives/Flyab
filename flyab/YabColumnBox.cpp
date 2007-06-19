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

void YabColumnBox::RemoveAll()
{	
	for(int i=1; i<myrows.size(); i++)
	{
		myrows[i].clear();
		myrows.pop_back();
	}
}

const char* YabColumnBox::ItemAt(int column, int position)
{
	return myrows[position][column-1].c_str();
}

int YabColumnBox::SelectedRow()
{
	for(int i=0; i<myrows.size()-1; i++)
		if(row_selected(i)) return i+1;
	return 0;
}

int YabColumnBox::NumRows()
{
	return myrows.size()-1;
}

int YabColumnBox::NumColumns()
{
	if(myrows.size() != 0)
		return myrows[0].size();
	return 0;
}

void YabColumnBox::SetColor(int col, int r, int g, int b)
{
	switch(col)
	{
		case 1:
			selectionTextColor = fl_rgb_color(r,g,b);
			break;
		case 2:	
			break;
		case 3:
			selection_color(fl_rgb_color(r,g,b));
			break;
		case 4:
			textColor = fl_rgb_color(r,g,b);
			break;
		case 5:
			rowDividerColor = fl_rgb_color(r,g,b);
			break;
		case 6:
			backgroundColor = fl_rgb_color(r,g,b);
			color(backgroundColor);
			break;
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
				fl_draw_box(FL_THIN_UP_BOX, X, Y, W, H, fl_rgb_color(B_GREY)); //color());
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
				fl_color( row_selected(R) ? selection_color() : backgroundColor);
				fl_rectf(X, Y, W, H);
				// TEXT
				fl_color(row_selected(R) ? selectionTextColor: textColor);
				fl_draw(myrows[R+1][C].c_str(), X, Y, W, H, alignments[C]);
				// BORDER
				fl_color(rowDividerColor); //FL_LIGHT2); 
				// fl_rect(X, Y, W, H);
				fl_line(X, Y+H-1, X+W, Y+H-1);
    			}
			fl_pop_clip();
			return;
		}

		default:
			return;
	}
}

