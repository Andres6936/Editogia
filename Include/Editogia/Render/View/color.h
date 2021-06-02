#ifndef _COLOR_H_
#define _COLOR_H_

#include <string>

void init_colors();

enum EColor
{
	c_black = 0,
	c_ltgray,
	c_red,
	c_green,
	c_blue,
	c_cyan,
	c_magenta,
	c_brown,
// "Bright" colors
	c_dkgray,
	c_white,
	c_ltred,
	c_ltgreen,
	c_ltblue,
	c_ltcyan,
	c_pink,
	c_yellow,
	c_null
};

long get_color_pair(EColor fg, EColor bg);

void extract_colors(long color, long attributes,
		EColor& fg, EColor& bg);

EColor color_string(std::string id);

std::string color_name(EColor color);

std::string color_tag(EColor color);

EColor hilight(EColor orig);

EColor non_bright(EColor col);

#endif
