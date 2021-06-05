#include "Editogia/Render/View/color.h"

#if (defined _WIN32 || defined WINDOWS)
#include "Editogia/catacurse.h"
#else

#include <curses.h>

#endif

long get_color_pair(EColor fg, EColor bg)
{
	if (fg == c_null)
		fg = c_ltgray;
	if (bg == c_null)
		bg = c_black;

	int pairnum = int(fg) % 8 + 1 + 8 * (int(bg) % 8);
	long ret = COLOR_PAIR(pairnum);
	if (fg >= c_dkgray)
		ret |= A_BOLD;
	if (bg >= c_dkgray)
		ret |= A_BLINK;

	return ret;
}

EColor color_string(std::string id)
{
	if (id == "black")
		return c_black;
	if (id == "grey" || id == "gray" || id == "ltgray")
		return c_ltgray;
	if (id == "red")
		return c_red;
	if (id == "green")
		return c_green;
	if (id == "blue")
		return c_blue;
	if (id == "cyan")
		return c_cyan;
	if (id == "magenta")
		return c_magenta;
	if (id == "brown")
		return c_brown;
	if (id == "dkgray")
		return c_dkgray;
	if (id == "white")
		return c_white;
	if (id == "ltred")
		return c_ltred;
	if (id == "ltgreen")
		return c_ltgreen;
	if (id == "ltblue")
		return c_ltblue;
	if (id == "ltcyan")
		return c_ltcyan;
	if (id == "pink")
		return c_pink;
	if (id == "yellow")
		return c_yellow;

	return c_null;
}

std::string color_name(EColor color)
{
	switch (color)
	{
	case c_black:
		return "Black";
	case c_ltgray:
		return "Light Gray";
	case c_red:
		return "Red";
	case c_green:
		return "Green";
	case c_blue:
		return "Blue";
	case c_cyan:
		return "Cyan";
	case c_magenta:
		return "Magenta";
	case c_brown:
		return "Brown";
	case c_dkgray:
		return "Dark Gray";
	case c_white:
		return "White";
	case c_ltred:
		return "Light Red";
	case c_ltgreen:
		return "Light Green";
	case c_ltblue:
		return "Light Blue";
	case c_ltcyan:
		return "Light Cyan";
	case c_pink:
		return "Pink";
	case c_yellow:
		return "Yellow";
	case c_null:
		return "Unchanged";
	default:
		return "???";
	}
	return "???";
}

std::string color_tag(EColor color)
{
	switch (color)
	{
	case c_black:
		return "black";
	case c_ltgray:
		return "ltgray";
	case c_red:
		return "red";
	case c_green:
		return "green";
	case c_blue:
		return "blue";
	case c_cyan:
		return "cyan";
	case c_magenta:
		return "magenta";
	case c_brown:
		return "brown";
	case c_dkgray:
		return "dkgray";
	case c_white:
		return "white";
	case c_ltred:
		return "ltred";
	case c_ltgreen:
		return "ltgreen";
	case c_ltblue:
		return "ltblue";
	case c_ltcyan:
		return "ltcyan";
	case c_pink:
		return "pink";
	case c_yellow:
		return "yellow";
	case c_null:
		return "/";
	default:
		return "/";
	}
	return "/";
}


EColor hilight(EColor orig)
{
	if (orig < c_dkgray)
		return EColor(int(orig) + 8);
	else
		return EColor(int(orig) - 8);
}

EColor non_bright(EColor col)
{
	switch (col)
	{
	case c_dkgray:
		return c_ltgray; // Don't want to use black
	case c_white:
		return c_ltgray;
	case c_ltred:
		return c_red;
	case c_ltgreen:
		return c_green;
	case c_ltblue:
		return c_blue;
	case c_ltcyan:
		return c_cyan;
	case c_pink:
		return c_magenta;
	case c_yellow:
		return c_brown;
	default:
		return col;
	}
	return c_null;
}