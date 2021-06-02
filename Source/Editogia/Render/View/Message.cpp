// Joan Andrés (@Andres6936) Github.

#include <sstream>
#include "Editogia/Render/View/window.h"
#include "Editogia/Render/View/Message.hpp"


nc_color message_type_color(Message_type type)
{
	switch (type)
	{
	case MESSAGE_NULL:
		return c_dkgray;
	case MESSAGE_MINOR:
		return c_ltgray;
	case MESSAGE_MAJOR:
		return c_yellow;
	case MESSAGE_URGENT:
		return c_ltred;
	case MESSAGE_UNLOCK:
		return c_ltgreen;
	case MESSAGE_MAX:
		return c_dkgray;
	default:
		return c_magenta;
	}
	return c_magenta;
}


std::string Message::save_data()
{
	std::stringstream ret;
	ret << int(type) << " ";
	ret << date.save_data() << std::endl;
// Since text is probably multi-word, we use ! as a terminator.
	ret << text << " ! ";

	return ret.str();
}

bool Message::load_data(std::istream& data)
{
	int tmptype;
	data >> tmptype;
	if (tmptype <= 0 || tmptype >= MESSAGE_MAX)
	{
		debugmsg("Message loaded type %d (range is 1 to %d).",
				tmptype, MESSAGE_MAX - 1);
		return false;
	}
	type = Message_type(tmptype);

	if (!date.load_data(data))
	{
		debugmsg("Message failed to load its date.");
		return false;
	}

	std::string word;
	while (word != "!")
	{
		data >> word;
		if (word != "!")
		{
			if (!text.empty())
			{
				text = text + " ";
			}
			text = text + word;
		}
	}

	return true;
}

