// Joan Andrés (@Andres6936) Github.

#include "Cuss/ActionID.hpp"


bool action_needs_element(action_id act)
{
	return (act == ACT_SELECT_STR || act == ACT_SCROLL || act == ACT_SET_COLORS ||
			act == ACT_TRANSLATE);
}

std::string action_name(action_id act)
{
	switch (act)
	{

	case ACT_NULL:
		return "Nothing";

	case ACT_SELECT_NEXT:
		return "Select Next";
	case ACT_SELECT_LAST:
		return "Select Last";
	case ACT_SELECT_NONE:
		return "Select None";
	case ACT_SELECT_STR:
		return "Select Element";

	case ACT_SCROLL:
		return "Scroll";

	case ACT_SET_COLORS:
		return "Set Colors";
	case ACT_TRANSLATE:
		return "Translate";

	default:
		return "Oops we forgot to name this";

	}
	return "What the heck?!";
}

