// Joan Andrés (@Andres6936) Github.

#include <sstream>
#include "Editogia/Util/stringfunc.h"
#include "Cuss/Elements/TextEntry.hpp"

#define SELECTCOLOR c_blue

using namespace cuss;

// *** TEXT ENTRY ELEMENT ***
void ele_textentry::draw(Window* win)
{
	EColor hilite = (selected ? SELECTCOLOR : bg);
// Ensure we see the end of the word--and a blank space
	int start = (selected ? text->size() + 1 - sizex : 0);
	if (start < 0)
		start = 0;
	int length = (selected ? sizex - 1 : sizex);

	std::string print = text->substr(start, length);

	win->putstr_raw(posx, posy, fg, hilite, print);
	for (int x = posx + print.length(); x < posx + sizex; x++)
		win->putch(x, posy, bg, hilite, '_');
}

std::string ele_textentry::save_data()
{
	std::stringstream ret;
	ret << Element::save_data() << " " << (*text) << " " << STD_DELIM;
	return ret.str();
}

void ele_textentry::load_data(std::istream& datastream)
{
	Element::load_data(datastream);
	(*text) = load_to_delim(datastream, STD_DELIM);
}

bool ele_textentry::self_reference()
{
	if (owns_data)
		return false;

	text = new std::string;
	owns_data = true;
	return true;
}

bool ele_textentry::set_data(std::string data)
{
	(*text) = data;
	return true;
}

bool ele_textentry::add_data(std::string data)
{
	(*text) += data;
	return true;
}

bool ele_textentry::ref_data(std::string* data)
{
	if (owns_data)
		delete text;

	text = data;
	owns_data = false;
	return true;
}

bool ele_textentry::handle_keypress(long ch)
{
	if (is_backspace(ch) && !text->empty())
	{
		(*text) = text->substr(0, text->length() - 1);
		return true;
	}
	if (ch >= 32 && ch <= 126)
	{ // Printable chars
		(*text) += char(ch);
		return true;
	}
	return false;
}

