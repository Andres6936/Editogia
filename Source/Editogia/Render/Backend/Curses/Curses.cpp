// Joan Andrés (@Andres6936) Github.

#include "Editogia/Render/Backend/Curses/Curses.hpp"

using namespace Editogia;

void Curses::writeChar(const std::int32_t x, const std::int32_t y, const std::int32_t _char,
		const IRender::Color foreground, const IRender::Color background)
{
	const std::int64_t colorValue = get_color_pair(foreground, background);

	wattron(window, colorValue);
	mvwaddch(window, y, x, _char);
	wattroff(window, colorValue);
}
