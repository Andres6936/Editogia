// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_MENU_HPP
#define EDITOGIA_MENU_HPP

#include <vector>
#include <string>

enum class TypeMenu
{
	None,
	Game,
	Help,
	World,
	Buildings,
	Ministers,
};

struct Menu
{
	std::string name;
	TypeMenu id;
	int posx;
	std::vector<std::string> items;
};

#endif //EDITOGIA_MENU_HPP
