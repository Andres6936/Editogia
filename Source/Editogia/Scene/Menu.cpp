// Joan Andrés (@Andres6936) Github.

#include "Editogia/Scene/Menu.hpp"

const char* toString(const TypeMenu type) noexcept
{
	switch (type)
	{
	case TypeMenu::None:
		return "None";
	case TypeMenu::Game:
		return "Game";
	case TypeMenu::Ministers:
		return "Ministers";
	case TypeMenu::Buildings:
		return "Buildings";
	case TypeMenu::World:
		return "World";
	case TypeMenu::Help:
		return "Help";
	default:
		return "Error";
	}
}

