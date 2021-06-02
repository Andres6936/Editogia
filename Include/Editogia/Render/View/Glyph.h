#ifndef _GLYPH_H_
#define _GLYPH_H_

#include "Editogia/Render/View/color.h"
#include <istream>

#define HILITE_COLOR c_blue
#define NO_BRIGHT_BG true

struct Glyph
{
	long symbol;
	EColor fg;
	EColor bg;

	Glyph()
	{
		symbol = 'x';
		fg = c_red;
		bg = c_white;
	}

	Glyph(long S, EColor F, EColor B) : symbol(S), fg(F), bg(B)
	{
	}

	Glyph invert();

	Glyph hilite(EColor back = HILITE_COLOR);

	std::string text_formatted();

	bool operator==(const Glyph& rhs);

	std::string save_data();

	void load_data(std::istream& datastream);

};

#endif
