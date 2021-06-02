#include <sstream>
#include "Editogia/Render/View/Glyph.h"
#include "Editogia/Render/View/window.h"

std::string Glyph::save_data()
{
	std::stringstream ret;
	ret << symbol << " " << int(fg) << " " << int(bg);
	return ret.str();
};

void Glyph::load_data(std::istream& datastream)
{
	int fgtmp, bgtmp;
	datastream >> symbol >> fgtmp >> bgtmp;

	fg = EColor(fgtmp);
	bg = EColor(bgtmp);
}

Glyph Glyph::invert()
{
	Glyph ret = (*this);
	EColor tmp = ret.fg;
	ret.fg = ret.bg;
	ret.bg = tmp;
	if (NO_BRIGHT_BG)
	{
		ret.bg = non_bright(ret.bg);
	}
	return ret;
}

Glyph Glyph::hilite(EColor back)
{
	if (fg == back)
	{
		return invert();
	}
	Glyph ret = (*this);

	if (ret.bg == back)
	{
		ret.bg = c_black;
	}
	else
	{
		ret.bg = back;
	}
	return ret;
}

std::string Glyph::text_formatted()
{
	std::stringstream ret;
	ret << "<c=" << color_tag(fg) << "," << color_tag(bg) << ">" <<
		char(symbol) << "<c=/>";
	return ret.str();
}

bool Glyph::operator==(const Glyph& rhs)
{
	return (rhs.fg == fg && rhs.bg == bg && rhs.symbol == symbol);
}

