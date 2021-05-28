#include <sstream>
#include "Editogia/View/Glyph.h"
#include "Editogia/window.h"

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

	fg = nc_color(fgtmp);
	bg = nc_color(bgtmp);
}

Glyph Glyph::invert()
{
	Glyph ret = (*this);
	nc_color tmp = ret.fg;
	ret.fg = ret.bg;
	ret.bg = tmp;
	if (NO_BRIGHT_BG)
	{
		ret.bg = non_bright(ret.bg);
	}
	return ret;
}

Glyph Glyph::hilite(nc_color back)
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

