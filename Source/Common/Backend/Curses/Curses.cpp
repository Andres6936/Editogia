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

void Curses::draw()
{
	wrefresh(window);
}

void Curses::clear()
{
	werase(window);
}

const std::int32_t Curses::getCharacterAt(const std::int32_t x, const std::int32_t y)
{
	return mvwinch(window, y, x) & A_CHARTEXT;
}

const IRender::Color Curses::getForegroundColorAt(const std::int32_t x, const std::int32_t y)
{
	const std::uint64_t foregroundColor = mvwinch(window, y, x) & A_COLOR;
	const std::uint64_t foregroundAttributes = mvwinch(window, y, x) & A_ATTRIBUTES;

	for (std::size_t index = 1; index < 256; ++index)
	{
		if (foregroundColor == COLOR_PAIR(index))
		{
			if (foregroundAttributes & A_BOLD)
			{
				return EColor(((index - 1) % 8) + 8);
			}

			return EColor((index - 1) % 8);
		}
	}

	return c_black;
}

const IRender::Color Curses::getBackgroundColorAt(const std::int32_t x, const std::int32_t y)
{
	const std::uint64_t backgroundColor = mvwinch(window, y, x) & A_COLOR;
	const std::uint64_t backgroundAttributes = mvwinch(window, y, x) & A_ATTRIBUTES;

	for (std::size_t index = 1; index < 256; ++index)
	{
		if (backgroundColor == COLOR_PAIR(index))
		{
			if (backgroundAttributes & A_BLINK)
			{
				return EColor(((index - 1) / 8) + 8);
			}

			return EColor((index - 1) / 8);
		}
	}

	return c_black;
}

void Curses::writeChar(const std::int32_t x, const std::int32_t y, const std::int32_t _char,
		const IRender::Color foreground, const IRender::Color background)
{
	const std::int64_t colorValue = get_color_pair(foreground, background);

	wattron(window, colorValue);
	mvwaddch(window, y, x, _char);
	wattroff(window, colorValue);
}
