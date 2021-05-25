// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_MESSAGE_HPP
#define EDITOGIA_MESSAGE_HPP

#include <string>
#include "Editogia/date.h"
#include "Editogia/color.h"

enum Message_type
{
	MESSAGE_NULL = 0,
	MESSAGE_MINOR,    // Player MIGHT want to take action on this
	MESSAGE_MAJOR,    // Player will almost certainly want to take action on this
	MESSAGE_URGENT,   // Player MUST take action on this!
	MESSAGE_UNLOCK,    // We've unlocked a new area, building, or other feature
	MESSAGE_MAX
};

// Color-coding for the different message types.
nc_color message_type_color(Message_type type);

struct Message
{
	Message(Message_type MT = MESSAGE_MINOR, std::string T = "") :
			type(MT), text(T)
	{
	}

	std::string save_data();

	bool load_data(std::istream& data);

	Message_type type;
	std::string text;
	Date date;
};

#endif //EDITOGIA_MESSAGE_HPP
