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

	// Getters

	const std::int32_t getWidth() const noexcept;

	const std::string& getName() const noexcept;

	const TypeMenu getTypeMenu() const noexcept;

	const std::vector<std::string>& getItems() const noexcept;
};

#endif //EDITOGIA_MENU_HPP
