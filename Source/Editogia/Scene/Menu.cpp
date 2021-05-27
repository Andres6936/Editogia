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

// Getters

const std::string& Menu::getName() const noexcept
{
	return name;
}

const TypeMenu Menu::getTypeMenu() const noexcept
{
	return typeMenu;
}

const std::int32_t Menu::getCoordinateX() const noexcept
{
	return coordinateX;
}

const std::vector<std::string>& Menu::getItems() const noexcept
{
	return items;
}
