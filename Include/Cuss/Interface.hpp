// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_INTERFACE_HPP
#define EDITOGIA_INTERFACE_HPP

#include <map>
#include <string>
#include <vector>
#include "Editogia/window.h"
#include "Editogia/geometry.h"
#include "Cuss/Binding.hpp"
#include "Cuss/ActionID.hpp"
#include "Cuss/Elements/Type.hpp"
#include "Cuss/Elements/Element.hpp"

namespace cuss
{

	/**
	 * Constraints: DefaultConstructible.
	 */
	class interface
	{

	private:

		// Private properties

		int active_element = -1;

		std::vector<element*> elements;

		std::map<long, binding> bindings;

	public:

		// Public properties

		int sizey = 24;

		int sizex = 80;

		/**
		 * The name of interface.
		 */
		std::string name;

		// Constructor

		/**
		 * Default constructors.
		 */
		interface() noexcept = default;

		interface(std::string name, int width, int height) noexcept;

		~interface();

		void add_element(element_type type, std::string name, int posx, int posy,
				int sizex, int sizey, bool selectable = true);

		bool erase_element(std::string name);

		void draw(Window* win);

		void draw_prototype(Window* win); // For the editor

		std::string save_data();

		void load_data(std::istream& datastream);

		bool load_from_file(std::string filename, bool warn = true);

		std::vector <std::string> element_names();

		element* selected();

		element* find_by_name(std::string name);

		element* select_next(bool force = false);

		element* select_last(bool force = false);

		element* select(std::string name);

		void select_none();

		bool move_element_up(std::string name);

		bool move_element_down(std::string name);

		bool set_selectable(std::string name, bool setting);

		// set_data replaces the element's data with whatever is passed
		// add_data appends whatever is passed to the element's data
		// These are all defined for each element type; if they're invalid, the type
		//  just returns false.
		bool set_data(std::string name, std::string data);

		bool add_data(std::string name, std::string data);

		bool set_data(std::string name, std::vector<std::string> data);

		bool add_data(std::string name, std::vector<std::string> data);

		bool set_data(std::string name, int data);

		bool add_data(std::string name, int data);

		bool set_data(std::string name, glyph gl, int posx, int posy);

		bool set_data(std::string name, nc_color fg, nc_color bg = c_null);

		/* self_reference makes an element take control of its own data (meaning it will
		 * delete the data when it is deleted).
		 * ref_data makes an element reference some external set of data.  Note that if
		 * a function is not appropriate, it will not change the reference and will
		 * return false (e.g. telling a number element to reference a vector of strings)
		 */
		bool self_reference(std::string name);

		bool ref_data(std::string name, std::string* data);

		bool ref_data(std::string name, std::vector<std::string>* data);

		bool ref_data(std::string name, int* data);

		bool clear_data(std::string name);

		// These will return empty data if inappropriate to the element.
		std::string get_str(std::string name);

		int get_int(std::string name);

		int element_width(std::string name);

		std::vector<std::string> binding_list();

		bool add_binding(long ch, action_id act, std::string target,
				int a, int b = 0);

		binding* bound_to(long ch);

		bool has_bindings_for(action_id act);

		bool has_bindings_for(std::string target);

		bool rem_binding(long ch);

		bool rem_all_bindings(action_id act = ACT_NULL);

		bool rem_all_bindings(std::string target);

		// Only does keybindings
		bool handle_action(long ch);

		// May redirect to current object
		bool handle_keypress(long ch);
	};
}

#endif //EDITOGIA_INTERFACE_HPP
