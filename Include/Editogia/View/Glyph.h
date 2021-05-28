#ifndef _GLYPH_H_
#define _GLYPH_H_

#include "Editogia/color.h"
#include <istream>

#define HILITE_COLOR c_blue
#define NO_BRIGHT_BG true

struct Glyph
{
	long symbol;
	nc_color fg;
	nc_color bg;

	Glyph()
	{
		symbol = 'x';
		fg = c_red;
		bg = c_white;
	}

	Glyph(long S, nc_color F, nc_color B) : symbol(S), fg(F), bg(B)
	{
	}

	Glyph invert();

	Glyph hilite(nc_color back = HILITE_COLOR);

	std::string text_formatted();

	bool operator==(const Glyph& rhs);

	std::string save_data();

	void load_data(std::istream& datastream);

};

#endif
