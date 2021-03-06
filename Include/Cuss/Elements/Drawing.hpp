// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_DRAWING_HPP
#define EDITOGIA_DRAWING_HPP

#include <map>
#include "Editogia/Math/geometry.h"
#include "Cuss/Elements/Element.hpp"

namespace cuss
{

	struct ele_drawing : public Element
	{
		std::map<Point, Glyph, Pointcomp> drawing;

		ele_drawing()
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
		}

		virtual element_type type()
		{
			return ELE_DRAWING;
		};

		virtual void draw(Window* win);

		virtual std::string save_data();

		virtual void load_data(std::istream& datastream);

		virtual bool set_data(Glyph gl, int posx, int posy);

		virtual bool set_data(EColor FG, EColor BG = c_null);

		virtual bool handle_keypress(long ch)
		{
			return false;
		};

		virtual void clear_data()
		{
			drawing.clear();
		};

		/* Translate is breaking a rule here; it's a function that isn't inherited from
		 * Element.  I'm not sure I'm okay with this, but for now I think that translate
		 * is really drawing-specific; it looks for all instances of "from", and moves
		 * them to "to".  While things like textbox could probably use this, I'm holding
		 * off for now.
		 */
		bool translate(long from, long to);
	};
}

#endif //EDITOGIA_DRAWING_HPP
