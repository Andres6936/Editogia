#include "Editogia/cuss.h"
#include "Editogia/stringfunc.h"
#include <sstream>
#include <fstream>

using namespace cuss;

void print_scrollbar(Window* win, int posx, int posy, int length, int offset,
		int size, bool selected);


#define SELECTCOLOR c_blue

#define PREP_ELEMENT(ele) \
  (ele)->name = name;\
  (ele)->posx = posx;\
  (ele)->posy = posy;\
  (ele)->sizex = szx;\
  (ele)->sizey = szy;\
  (ele)->selected = false;\
  (ele)->selectable = selectable;\
  (ele)->align = ALIGN_LEFT








void print_scrollbar(Window* win, int posx, int posy, int length, int offset,
		int size, bool selected)
{
	nc_color barcol = (selected ? SELECTCOLOR : c_ltgray);
	int barsize = (length >= size ? -1 : 1 + ((length * length) / size));

	if (barsize == -1)
	{
// Don't print a scroll bar
		for (int y = posy; y < posy + length; y++)
			win->putch(posx, y, barcol, c_black, LINE_XOXO);
	}
	else
	{
		int barpos = (offset * length) / size;
		if (barpos + barsize > length)
			barpos = length - barsize;
		for (int y = 0; y < length; y++)
		{
			long ch = ((y >= barpos && y < barpos + barsize) ? '#' : LINE_XOXO);
			win->putch(posx, posy + y, barcol, c_black, ch);
		}
	}
}



