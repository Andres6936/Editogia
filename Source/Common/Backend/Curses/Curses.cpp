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
	Key key{};

	const std::int64_t keyPressed = getch();

	if (keyPressed == 'a' || keyPressed == 'A')
	{
		key.setKeyCode(KeyCode::A);
	}
	else if (keyPressed == 'b' || keyPressed == 'B')
	{
		key.setKeyCode(KeyCode::B);
	}
	else if (keyPressed == 'c' || keyPressed == 'C')
	{
		key.setKeyCode(KeyCode::C);
	}
	else if (keyPressed == 'd' || keyPressed == 'D')
	{
		key.setKeyCode(KeyCode::D);
	}
	else if (keyPressed == 'e' || keyPressed == 'E')
	{
		key.setKeyCode(KeyCode::E);
	}
	else if (keyPressed == 'f' || keyPressed == 'F')
	{
		key.setKeyCode(KeyCode::F);
	}
	else if (keyPressed == 'g' || keyPressed == 'G')
	{
		key.setKeyCode(KeyCode::G);
	}
	else if (keyPressed == 'h' || keyPressed == 'H')
	{
		key.setKeyCode(KeyCode::H);
	}
	else if (keyPressed == 'i' || keyPressed == 'I')
	{
		key.setKeyCode(KeyCode::I);
	}
	else if (keyPressed == 'j' || keyPressed == 'J')
	{
		key.setKeyCode(KeyCode::J);
	}
	else if (keyPressed == 'k' || keyPressed == 'K')
	{
		key.setKeyCode(KeyCode::K);
	}
	else if (keyPressed == 'l' || keyPressed == 'L')
	{
		key.setKeyCode(KeyCode::L);
	}
	else if (keyPressed == 'm' || keyPressed == 'M')
	{
		key.setKeyCode(KeyCode::M);
	}
	else if (keyPressed == 'n' || keyPressed == 'N')
	{
		key.setKeyCode(KeyCode::N);
	}
	else if (keyPressed == 'o' || keyPressed == 'O')
	{
		key.setKeyCode(KeyCode::O);
	}
	else if (keyPressed == 'p' || keyPressed == 'P')
	{
		key.setKeyCode(KeyCode::P);
	}
	else if (keyPressed == 'q' || keyPressed == 'Q')
	{
		key.setKeyCode(KeyCode::Q);
	}
	else if (keyPressed == 'r' || keyPressed == 'R')
	{
		key.setKeyCode(KeyCode::R);
	}
	else if (keyPressed == 's' || keyPressed == 'S')
	{
		key.setKeyCode(KeyCode::S);
	}
	else if (keyPressed == 't' || keyPressed == 'T')
	{
		key.setKeyCode(KeyCode::T);
	}
	else if (keyPressed == 'w' || keyPressed == 'W')
	{
		key.setKeyCode(KeyCode::W);
	}
	else if (keyPressed == 'x' || keyPressed == 'X')
	{
		key.setKeyCode(KeyCode::X);
	}
	else if (keyPressed == 'y' || keyPressed == 'Y')
	{
		key.setKeyCode(KeyCode::Y);
	}
	else if (keyPressed == 'z' || keyPressed == 'Z')
	{
		key.setKeyCode(KeyCode::Z);
	}

	return key;
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
