// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_ELETEXTENTRY_HPP
#define EDITOGIA_ELETEXTENTRY_HPP

#include "Cuss/Elements/Element.hpp"

struct ele_textentry : public element
{
	std::string* text;

	ele_textentry()
	{
		name = "";
		posx = 0;
		posy = 0;
		sizex = 0;
		sizey = 0;
		selected = false;
		selectable = false;
		fg = c_ltgray;
		bg = c_black;
		owns_data = false;
		self_reference();
	}

	virtual element_type type()
	{
		return ELE_TEXTENTRY;
	};

	virtual void draw(Window* win);

	virtual std::string save_data();

	virtual void load_data(std::istream& datastream);

	virtual bool self_reference();

	virtual bool set_data(std::string data);

	virtual bool add_data(std::string data);

	virtual bool ref_data(std::string* data);

	virtual bool handle_keypress(long ch);

	virtual void clear_data()
	{
		text->clear();
	};

	virtual std::string get_str()
	{
		return (*text);
	};
};

#endif //EDITOGIA_ELETEXTENTRY_HPP
