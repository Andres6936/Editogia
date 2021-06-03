// Joan Andrés (@Andres6936) Github.

#include "Editogia/Render/Backend/Curses/Curses.hpp"

using namespace Editogia;

Curses::Curses() noexcept: Curses(0, 0)
{
	// Delegate the construction to parent construct
}

Curses::Curses(const std::int32_t width, const std::int32_t height) noexcept
		: Curses(0, 0, width, height)
{
	// Delegate the construction to parent construct
}

Curses::Curses(const std::int32_t x, const std::int32_t y, const std::int32_t width,
		const std::int32_t height) noexcept: IRender()
{
	// Call to construct of IRender

	window = newwin(height, width, y, x);
}

Curses::~Curses() noexcept
{
	delwin(window);
}

void Curses::writeChar(const std::int32_t x, const std::int32_t y, const std::int32_t _char,
		const IRender::Color foreground, const IRender::Color background)
{
	const std::int64_t colorValue = get_color_pair(foreground, background);

	wattron(window, colorValue);
	mvwaddch(window, y, x, _char);
	wattroff(window, colorValue);
}
