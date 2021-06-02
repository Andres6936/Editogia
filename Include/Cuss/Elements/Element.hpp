// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_ELEMENT_HPP
#define EDITOGIA_ELEMENT_HPP

#include <string>
#include <vector>
#include <cstdint>

#include "Editogia/Render/View/Glyph.h"
#include "Editogia/Render/View/color.h"
#include "Editogia/Render/View/window.h"
#include "Cuss/Elements/Type.hpp"
#include "Cuss/Elements/Alignment.hpp"

namespace cuss
{

	struct element
	{
		std::string name;
		int posx;
		int posy;
		int sizex;
		int sizey;
		bool selected;
		bool selectable;
		bool owns_data;
		EColor fg;
		EColor bg;
		alignment align;
		vertical_alignment v_align;

		bool recently_selected;

		// Setters

		void setName(const std::string& name) noexcept;

		void setWidth(const std::int32_t width) noexcept;

		void setHeight(const std::int32_t height) noexcept;

		void setSelected(const bool isSelected) noexcept;

		void setSelectable(const bool isSelectable) noexcept;

		void setCoordinateX(const std::int32_t x) noexcept;

		void setCoordinateY(const std::int32_t y) noexcept;

		void setHAlignment(const alignment HAlignment) noexcept;

		void setVAlignment(const vertical_alignment VAlignment) noexcept;

		element()
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
			owns_data = true;
			recently_selected = false;
		}

		virtual ~element()
		{
		}

		virtual element_type type()
		{
			return ELE_NULL;
		}

		virtual void draw(Window* win)
		{
		}

		virtual std::string save_data();

		virtual void load_data(std::istream& datastream);

		virtual bool self_reference()
		{
			return false;
		}

		virtual bool set_data(std::string data)
		{
			return false;
		}

		virtual bool add_data(std::string data)
		{
			return false;
		}

		virtual bool ref_data(std::string* data)
		{
			return false;
		}

		virtual bool set_data(std::vector <std::string> data)
		{
			return false;
		}

		virtual bool add_data(std::vector <std::string> data)
		{
			return false;
		}

		virtual bool ref_data(std::vector <std::string>* data)
		{
			return false;
		}

		virtual bool set_data(int data)
		{
			return false;
		}

		virtual bool add_data(int data)
		{
			return false;
		}

		virtual bool ref_data(int* data)
		{
			return false;
		}

		virtual bool set_data(Glyph gl, int posx, int posy)
		{
			return false;
		}

		virtual bool handle_keypress(long ch)
		{
			return false;
		}

		// This is used to set fg & bg, and hence is defined for element!
		virtual bool set_data(EColor FG, EColor BG = c_null);

		virtual bool set_alignment(alignment al)
		{
			align = al;
			return true;
		}

		virtual void clear_data()
		{
		}

		virtual std::string get_str()
		{
			std::string ret;
			return ret;
		}

		virtual int get_int()
		{
			return 0;
		}

		virtual std::vector <std::string> get_str_list()
		{
			std::vector <std::string> ret;
			return ret;
		}
	};
}

#endif //EDITOGIA_ELEMENT_HPP
