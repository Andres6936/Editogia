// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_MENU_HPP
#define EDITOGIA_MENU_HPP

#include <vector>
#include <string>

enum class TypeMenu
{
	None,
	Game,
	Ministers,
	Buildings,
	World,
	Help,
};

const char* toString(const TypeMenu type) noexcept;

struct Menu
{
	int width;
	std::string name;
	TypeMenu typeMenu;
	std::vector<std::string> items;
};

#endif //EDITOGIA_MENU_HPP
