// Joan Andrés (@Andres6936) Github.

#include <sstream>
#include "Editogia/Util/stringfunc.h"
#include "Cuss/Elements/Element.hpp"

using namespace cuss;

// Base save/load functions.
std::string element::save_data()
{
	std::stringstream ret;
	ret << name << " " << STD_DELIM << " " << posx << " " << posy << " " <<
		sizex << " " << sizey << " " << selectable << " " << int(align) << " " <<
		v_align;
	return ret.str();
}

void element::load_data(std::istream& datastream)
{
	name = load_to_delim(datastream, STD_DELIM);
	int tmpalign, tmpvalign;

	datastream >> posx >> posy >> sizex >> sizey >> selectable >> tmpalign >>
			   tmpvalign;
	align = alignment(tmpalign);
	v_align = vertical_alignment(tmpvalign);
}

bool element::set_data(EColor FG, EColor BG)
{
	if (fg != c_null)
		fg = FG;
	if (BG != c_null) // bg defaults to c_null
		bg = BG;

	return true;
}

// Setters

void element::setName(const std::string& _name) noexcept
{
	name = _name;
}

void element::setWidth(const std::int32_t width) noexcept
{
	sizex = width;
}

void element::setHeight(const std::int32_t height) noexcept
{
	sizey = height;
}

void element::setSelected(const bool isSelected) noexcept
{
	selected = isSelected;
}

void element::setSelectable(const bool isSelectable) noexcept
{
	selectable = isSelectable;
}

void element::setCoordinateX(const std::int32_t x) noexcept
{
	posx = x;
}

void element::setCoordinateY(const std::int32_t y) noexcept
{
	posy = y;
}

void element::setHAlignment(const alignment HAlignment) noexcept
{
	align = HAlignment;
}

void element::setVAlignment(const vertical_alignment VAlignment) noexcept
{
	v_align = VAlignment;
}
