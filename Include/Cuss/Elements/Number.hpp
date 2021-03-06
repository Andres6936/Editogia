// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_ELENUMBER_HPP
#define EDITOGIA_ELENUMBER_HPP

#include <string>
#include "Cuss/Elements/Element.hpp"

namespace cuss
{

	struct ele_number : public Element
	{
		int* value;

		ele_number()
		{
			name = "";
			posx = 0;
			posy = 0;
			sizex = 0;
			sizey = 0;
			selected = false;
			selectable = false;
			value = 0;
			fg = c_ltgray;
			bg = c_black;
			owns_data = false;
			self_reference();
		}

		virtual element_type type()
		{
			return ELE_NUMBER;
		};

		virtual void draw(Window* win);

		virtual std::string save_data();

		virtual void load_data(std::istream& datastream);

		virtual bool self_reference();

		virtual bool set_data(int data);

		virtual bool add_data(int data);

		virtual bool ref_data(int* data);

		virtual bool handle_keypress(long ch);

		virtual void clear_data()
		{
			(*value) = 0;
		};

		virtual int get_int()
		{
			return (*value);
		};
	};
}

#endif //EDITOGIA_ELENUMBER_HPP
