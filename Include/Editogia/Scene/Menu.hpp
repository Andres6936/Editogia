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

	/**
	 * @return The width in characters of menu.
	 */
	const std::int32_t getWidth() const noexcept;

	/**
	 * @return The name of menu.
	 */
	const std::string& getName() const noexcept;

	/**
	 * @return The type of menu.
	 */
	const TypeMenu getTypeMenu() const noexcept;

	/**
	 * @return The submenus of menu.
	 */
	const std::vector<std::string>& getItems() const noexcept;
};

#endif //EDITOGIA_MENU_HPP
