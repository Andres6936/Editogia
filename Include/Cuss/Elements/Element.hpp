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

	struct Element
	{
		std::string name;
		std::int32_t posx{ 0 };
		std::int32_t posy{ 0 };
		std::int32_t sizex{ 0 };
		std::int32_t sizey{ 0 };

		bool selected{ false };
		bool selectable{ false };
		bool owns_data{ true };
		bool recently_selected{ false };

		EColor fg = c_ltgray;
		EColor bg = c_black;
		alignment align;
		vertical_alignment v_align;

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

		// Constructor

		/**
		 * Default Constructor
		 */
		Element() noexcept = default;

		virtual ~Element()
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

		// This is used to set fg & bg, and hence is defined for Element!
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
