#include "Editogia/Entity/Org/city_achievement.h"
#include "Editogia/Entity/citizen.h"    // For citizen_type_name()
#include "Editogia/Util/stringfunc.h" // For capitalize()
#include <sstream>

std::string City_achievement::get_description()
{
	std::stringstream ret;

	switch (type)
	{

	case CITY_ACHIEVE_NULL:
		return "None";

	case CITY_ACHIEVE_ORES:
		return "Have tin, copper, or iron ore";

	case CITY_ACHIEVE_POP:
		ret << "Have " << value_b << " " <<
			citizen_type_name(Citizen_type(value_a), (value_b > 1));
		break;

	case CITY_ACHIEVE_MAX:
		return "BUG - CITY_ACHIEVE_MAX";

	default:
		return "BUG - Unknown City_achievement_type";

	}

	return capitalize(ret.str());
}
