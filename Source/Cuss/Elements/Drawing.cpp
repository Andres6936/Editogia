// Joan Andrés (@Andres6936) Github.

#include <sstream>
#include "Cuss/Elements/Drawing.hpp"

using namespace cuss;

// *** DRAWING ELEMENT ***
void ele_drawing::draw(Window* win)
{
	std::map<Point, Glyph>::iterator it;
	for (it = drawing.begin(); it != drawing.end(); it++)
		win->putglyph(posx + it->first.x, posy + it->first.y, it->second);
}

std::string ele_drawing::save_data()
{
	std::stringstream ret;
	ret << Element::save_data() << " " << drawing.size() << " ";
	std::map<Point, Glyph>::iterator it;
	for (it = drawing.begin(); it != drawing.end(); it++)
		ret << it->first.x << " " << it->first.y << " " << it->second.save_data() <<
			" ";
	return ret.str();
}

void ele_drawing::load_data(std::istream& datastream)
{
	Element::load_data(datastream);
	int tmpsize;
	datastream >> tmpsize;
	for (int i = 0; i < tmpsize; i++)
	{
		Point tmpPoint;
		Glyph tmpglyph;
		datastream >> tmpPoint.x >> tmpPoint.y;
		tmpglyph.load_data(datastream);
		drawing[tmpPoint] = tmpglyph;
	}
}

bool ele_drawing::set_data(Glyph gl, int posx, int posy)
{
	if (posx < 0 || posx >= sizex || posy < 0 || posy >= sizey)
		return false;

	if (gl.symbol == -1 || gl.symbol == ' ')
		drawing.erase(Point(posx, posy));
	else
		drawing[Point(posx, posy)] = gl;

	return true;
}

bool ele_drawing::set_data(EColor FG, EColor BG)
{
	std::map<Point, Glyph>::iterator it;
	for (it = drawing.begin(); it != drawing.end(); it++)
	{
		it->second.fg = FG;
		if (BG != c_null) // bg defaults to c_null
			it->second.bg = BG;
	}

	return true;
}

bool ele_drawing::translate(long from, long to)
{
	std::map<Point, Glyph>::iterator it;
	bool found = false;
	for (it = drawing.begin(); it != drawing.end(); it++)
	{
		if (it->second.symbol == from)
		{
			it->second.symbol = to;
			found = true;
		}
	}
	return found;
}

