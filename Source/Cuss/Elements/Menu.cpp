// Joan Andrés (@Andres6936) Github.

#include <sstream>
#include "Editogia/Util/stringfunc.h"
#include "Cuss/Elements/Menu.hpp"

#define SELECTCOLOR c_blue

using namespace cuss;

// *** MENU ELEMENT ***
void ele_menu::draw(Window* win)
{
	std::string text = title;
	if (selection >= 0)
	{
		text = (*list)[selection];
	}

	if (!selected || !open)
	{
		if (align == alignment::ALIGN_RIGHT)
		{
			win->putstr_r(posx + 1, posy, fg, (selected ? SELECTCOLOR : bg),
					sizex - 2, text);
		}
		else if (align == alignment::ALIGN_CENTER)
		{
			win->putstr_c(posx + 1, posy, fg, (selected ? SELECTCOLOR : bg),
					sizex - 2, text);
		}
		else
		{
			win->putstr_n(posx + 1, posy, fg, (selected ? SELECTCOLOR : bg),
					sizex - 2, text);
		}
		return;
	}
// The rest is for when it's selected, i.e. open
// Draw outline first
	int tmpsizey = list->size() + 2;
	if (posy + tmpsizey > win->sizey())
	{
		tmpsizey = win->sizey() - posy;
	}
// Vertical lines
	for (int y = posy + 1; y < posy + tmpsizey - 1; y++)
	{
		win->putch(posx, y, fg, bg, LINE_XOXO);
		win->putch(posx + sizex - 2, y, fg, bg, LINE_XOXO);
	}

// Horizontal lines
	for (int x = posx + 1; x < posx + sizex - 1; x++)
	{
		win->putch(x, posy, fg, bg, LINE_OXOX);
		win->putch(x, posy + tmpsizey - 1, fg, bg, LINE_OXOX);
	}

// Corners
	win->putch(posx, posy, fg, bg, LINE_OXXO);
	win->putch(posx + sizex - 2, posy, fg, bg, LINE_OOXX);
	win->putch(posx, posy + tmpsizey - 1, fg, bg, LINE_XXOO);
	win->putch(posx + sizex - 2, posy + tmpsizey - 1, fg, bg, LINE_XOOX);

// Then draw menu items
	if (align == alignment::ALIGN_RIGHT)
	{
		win->putstr_r(posx + 1, posy, fg, bg, sizex - 2, title);
	}
	else if (align == alignment::ALIGN_CENTER)
	{
		win->putstr_c(posx + 1, posy, fg, bg, sizex - 2, title);
	}
	else
	{
		win->putstr_n(posx + 1, posy, fg, bg, sizex - 2, title);
	}
	for (int i = 0; i < tmpsizey && i < list->size(); i++)
	{
		int n = i, line = i + posy + 1;
		if ((*list)[n] == "-")
		{ // Single dash indicates a horizontal line
			win->putch(posx, line, fg, bg, LINE_XXXO);
			win->putch(posx + sizex - 1, line, fg, bg, LINE_XOXX);
			for (int x = posx + 1; x < posx + sizex - 2; x++)
			{
				win->putch(x, line, fg, bg, LINE_OXOX);
			}
		}
		else
		{
// Clear the line using black Xs
			for (int x = posx + 1; x < posx + sizex - 2; x++)
			{
				win->putch(x, line, c_black, c_black, 'x');
			}
			EColor back = (n == selection ? SELECTCOLOR : bg);
			if (align == alignment::ALIGN_RIGHT)
			{
				win->putstr_r(posx + 1, line, fg, back, sizex - 2, (*list)[n]);
			}
			else if (align == alignment::ALIGN_CENTER)
			{
				win->putstr_c(posx + 1, line, fg, back, sizex - 2, (*list)[n]);
			}
			else
			{
				win->putstr_n(posx + 1, line, fg, back, sizex - 2, (*list)[n]);
			}
		}
	}
}

std::string ele_menu::save_data()
{
	std::stringstream ret;
	ret << Element::save_data() << " " << title << " " << STD_DELIM << " " <<
		list->size() << " ";
	for (int i = 0; i < list->size(); i++)
		ret << (*list)[i] << " " << STD_DELIM << " ";

	return ret.str();
}

void ele_menu::load_data(std::istream& datastream)
{
	Element::load_data(datastream);
	title = load_to_delim(datastream, STD_DELIM);
	int tmpsize;
	datastream >> tmpsize;
	for (int i = 0; i < tmpsize; i++)
	{
		std::string tmp = load_to_delim(datastream, STD_DELIM);
		list->push_back(tmp);
	}
}

bool ele_menu::self_reference()
{
	if (owns_data)
		return false;

	list = new std::vector<std::string>;
	owns_data = true;
	return true;
}

bool ele_menu::set_data(std::string data)
{
	title = data;
	return true;
}

bool ele_menu::add_data(std::string data)
{
	list->push_back(data);
	return true;
}

bool ele_menu::handle_keypress(long ch)
{
	if (ch == '\n')
	{
		open = false;
		return true;
	}
	return false;
}

bool ele_menu::set_data(std::vector <std::string> data)
{
	(*list) = data;
	selection = 0;
	open = false;
	return true;
}

bool ele_menu::add_data(std::vector <std::string> data)
{
	for (int i = 0; i < data.size(); i++)
		list->push_back(data[i]);
	return true;
}

bool ele_menu::ref_data(std::vector <std::string>* data)
{
	if (owns_data)
		delete list;

	list = data;
	owns_data = false;
	return true;
}

bool ele_menu::set_data(int data)
{
	selection = data;

	if (selection < 0)
		selection = 0;
	if (selection >= list->size())
		selection = list->size() - 1;

	if (data != -1)
	{
		open = true;
	}

	return true;
}

bool ele_menu::add_data(int data)
{
	return set_data(selection + data);
}

std::string ele_menu::get_str()
{
	if (selection < 0 || selection >= list->size())
	{
		std::string ret;
		return ret;
	}
	return (*list)[selection];
}

int ele_menu::get_int()
{
	return selection;
}

