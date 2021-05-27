// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_MENU_HPP
#define EDITOGIA_MENU_HPP

#include <vector>
#include <string>

enum Menu_id
{
	MENU_NULL = 0,
	MENU_GAME,
	MENU_MINISTERS,
	MENU_BUILDINGS,
	MENU_WORLD,
	MENU_HELP,
	MENU_MAX
};

struct Menu
{
	std::string name;
	Menu_id id;
	int posx;
	std::vector<std::string> items;
};

#endif //EDITOGIA_MENU_HPP
