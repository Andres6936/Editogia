// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_MENU_HPP
#define EDITOGIA_MENU_HPP

#include <vector>
#include <string>
#include <cstdint>

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
	/**
	 * The width of menu in characters.
	 */
	std::int32_t width;

	/**
	 * The name of menu.
	 */
	std::string name;

	/**
	 * The type of menu.
	 */
	TypeMenu typeMenu;

	/**
	 * The submenus of menu.
	 */
	std::vector<std::string> items;
};

#endif //EDITOGIA_MENU_HPP
