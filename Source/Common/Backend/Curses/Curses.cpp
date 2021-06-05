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

// Private methods

const std::pair<std::uint64_t, std::uint64_t>
Curses::getColorAt(const std::int32_t x, const std::int32_t y)
{
	return { mvwinch(window, y, x) & A_COLOR, mvwinch(window, y, x) & A_ATTRIBUTES };
}

// Public methods

void Curses::draw()
{
	wrefresh(window);
}

void Curses::clear()
{
	werase(window);
}

const Key Curses::getKeyEvent()
{
	return Key();
}

const std::int32_t Curses::getCharacterAt(const std::int32_t x, const std::int32_t y)
{
	return mvwinch(window, y, x) & A_CHARTEXT;
}

const IRender::Color Curses::getForegroundColorAt(const std::int32_t x, const std::int32_t y)
{
	const auto[color, attributes] = getColorAt(x, y);

	for (std::size_t index = 1; index < 64; ++index)
	{
		if (color == COLOR_PAIR(index))
		{
			if (attributes & A_BOLD)
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
	const auto[color, attributes] = getColorAt(x, y);

	for (std::size_t index = 1; index < 64; ++index)
	{
		if (color == COLOR_PAIR(index))
		{
			if (attributes & A_BLINK)
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
