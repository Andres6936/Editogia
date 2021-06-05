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

	// Lazy initialization of Backend, this block of code is execute only once
	if (not startUp)
	{
		initscr();
		noecho();
		cbreak();
		keypad(stdscr, true);
		startUpColorSchema();
		curs_set(0);
		timeout(1);
		getch();
		timeout(-1);

		// Avoid call this block code more of once more
		startUp = true;
	}

	window = newwin(height, width, y, x);
}

Curses::~Curses() noexcept
{
	delwin(window);
}

// Private methods

void Curses::startUpColorSchema()
{
	start_color();

	init_pair(1, COLOR_BLACK, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
	init_pair(5, COLOR_BLUE, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(8, COLOR_YELLOW, COLOR_BLACK);

	init_pair(9, COLOR_BLACK, COLOR_WHITE);
	init_pair(10, COLOR_WHITE, COLOR_WHITE);
	init_pair(11, COLOR_RED, COLOR_WHITE);
	init_pair(12, COLOR_GREEN, COLOR_WHITE);
	init_pair(13, COLOR_BLUE, COLOR_WHITE);
	init_pair(14, COLOR_CYAN, COLOR_WHITE);
	init_pair(15, COLOR_MAGENTA, COLOR_WHITE);
	init_pair(16, COLOR_YELLOW, COLOR_WHITE);

	init_pair(17, COLOR_BLACK, COLOR_RED);
	init_pair(18, COLOR_WHITE, COLOR_RED);
	init_pair(19, COLOR_RED, COLOR_RED);
	init_pair(20, COLOR_GREEN, COLOR_RED);
	init_pair(21, COLOR_BLUE, COLOR_RED);
	init_pair(22, COLOR_CYAN, COLOR_RED);
	init_pair(23, COLOR_MAGENTA, COLOR_RED);
	init_pair(24, COLOR_YELLOW, COLOR_RED);

	init_pair(25, COLOR_BLACK, COLOR_GREEN);
	init_pair(26, COLOR_WHITE, COLOR_GREEN);
	init_pair(27, COLOR_RED, COLOR_GREEN);
	init_pair(28, COLOR_GREEN, COLOR_GREEN);
	init_pair(29, COLOR_BLUE, COLOR_GREEN);
	init_pair(30, COLOR_CYAN, COLOR_GREEN);
	init_pair(31, COLOR_MAGENTA, COLOR_GREEN);
	init_pair(32, COLOR_YELLOW, COLOR_GREEN);

	init_pair(33, COLOR_BLACK, COLOR_BLUE);
	init_pair(34, COLOR_WHITE, COLOR_BLUE);
	init_pair(35, COLOR_RED, COLOR_BLUE);
	init_pair(36, COLOR_GREEN, COLOR_BLUE);
	init_pair(37, COLOR_BLUE, COLOR_BLUE);
	init_pair(38, COLOR_CYAN, COLOR_BLUE);
	init_pair(39, COLOR_MAGENTA, COLOR_BLUE);
	init_pair(40, COLOR_YELLOW, COLOR_BLUE);

	init_pair(41, COLOR_BLACK, COLOR_CYAN);
	init_pair(42, COLOR_WHITE, COLOR_CYAN);
	init_pair(43, COLOR_RED, COLOR_CYAN);
	init_pair(44, COLOR_GREEN, COLOR_CYAN);
	init_pair(45, COLOR_BLUE, COLOR_CYAN);
	init_pair(46, COLOR_CYAN, COLOR_CYAN);
	init_pair(47, COLOR_MAGENTA, COLOR_CYAN);
	init_pair(48, COLOR_YELLOW, COLOR_CYAN);

	init_pair(49, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(50, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(51, COLOR_RED, COLOR_MAGENTA);
	init_pair(52, COLOR_GREEN, COLOR_MAGENTA);
	init_pair(53, COLOR_BLUE, COLOR_MAGENTA);
	init_pair(54, COLOR_CYAN, COLOR_MAGENTA);
	init_pair(55, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(56, COLOR_YELLOW, COLOR_MAGENTA);

	init_pair(57, COLOR_BLACK, COLOR_YELLOW);
	init_pair(58, COLOR_WHITE, COLOR_YELLOW);
	init_pair(59, COLOR_RED, COLOR_YELLOW);
	init_pair(60, COLOR_GREEN, COLOR_YELLOW);
	init_pair(61, COLOR_BLUE, COLOR_YELLOW);
	init_pair(62, COLOR_CYAN, COLOR_YELLOW);
	init_pair(63, COLOR_MAGENTA, COLOR_YELLOW);
	init_pair(64, COLOR_YELLOW, COLOR_YELLOW);
}

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
	else if (keyPressed == '*')
	{
		key.setKeyCode(KeyCode::KP_MUL);
	}
	else if (keyPressed == '+')
	{
		key.setKeyCode(KeyCode::KP_ADD);
	}
	else if (keyPressed == '-')
	{
		key.setKeyCode(KeyCode::KP_SUB);
	}
	else if (keyPressed == '.')
	{
		key.setKeyCode(KeyCode::DOT);
	}
	else if (keyPressed == '<')
	{
		key.setKeyCode(KeyCode::LESS_THAN);
	}
	else if (keyPressed == '>')
	{
		key.setKeyCode(KeyCode::GREATER_THAN);
	}
	else if (keyPressed == '[')
	{
		key.setKeyCode(KeyCode::OPEN_BRACKET);
	}
	else if (keyPressed == ']')
	{
		key.setKeyCode(KeyCode::CLOSED_BRACKET);
	}
	else if (keyPressed == '!')
	{
		key.setKeyCode(KeyCode::EXCLAMATION_MARK);
	}
	else if (keyPressed == KEY_UP)
	{
		key.setKeyCode(KeyCode::UP);
	}
	else if (keyPressed == KEY_DOWN)
	{
		key.setKeyCode(KeyCode::DOWN);
	}
	else if (keyPressed == KEY_LEFT)
	{
		key.setKeyCode(KeyCode::LEFT);
	}
	else if (keyPressed == KEY_RIGHT)
	{
		key.setKeyCode(KeyCode::RIGHT);
	}
	else if (keyPressed == KEY_ENTER)
	{
		key.setKeyCode(KeyCode::ENTER);
	}
	else if (keyPressed == 27)
	{
		key.setKeyCode(KeyCode::ESCAPE);
	}

	// For avoid write a key.setPressed(true) in each if block,
	// if the user press a key, set the boolean in the key interface
	if (key.getKeyCode() not_eq KeyCode::NONE)
	{
		key.setPressed(true);
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
