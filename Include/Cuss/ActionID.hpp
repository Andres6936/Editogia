// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_ACTIONID_HPP
#define EDITOGIA_ACTIONID_HPP

enum action_id
{
	ACT_NULL = 0,    // Do nothing

	ACT_SELECT_NEXT, // Select next
	ACT_SELECT_LAST, // Select last
	ACT_SELECT_NONE, // Select nothing
	ACT_SELECT_STR,  // Select something specific

	ACT_SCROLL,      // Scroll or adjust int field

	ACT_SET_COLORS,  // Set foreground color
	ACT_TRANSLATE,   // Change char A to char B

	ACT_MAX
};

bool action_needs_element(action_id act);

std::string action_name(action_id act);

#endif //EDITOGIA_ACTIONID_HPP
