#include <vector>
#include <string>
#include <list>
#include <cstring>
#include <sstream>

#include "Cuss/Interface.hpp"
#include "Editogia/Render/View/window.h"
#include "Editogia/Render/View/TextColor.hpp"
#include "Editogia/Render/Backend/Curses/Curses.hpp"

std::vector<Editogia::TextColor> parseText(
		std::string text, const EColor foreground, const EColor background)
{
	using namespace Editogia;

	std::size_t tag;
	std::vector<TextColor> result;
	EColor cur_fg = foreground, cur_bg = background;

	while ((tag = text.find("<c=")) != std::string::npos)
	{
		// Everything before the tag is a segment, with the current colors
		result.push_back({ text.substr(0, tag), cur_fg, cur_bg });

		// Strip off everything up to and including "<c="
		text = text.substr(tag + 3);
		// Find the end of the tag
		size_t tagend = text.find(">");
		if (tagend == std::string::npos)
		{
			debugmsg("Unterminated color tag! %d:%s:", int(tag), text.c_str());
			throw std::logic_error("Unterminated color tag!");
		}

		std::string tag = text.substr(0, tagend);
		// Strip out the tag
		text = text.substr(tagend + 1);

		if (tag == "reset" || tag == "/")
		{
			// Reset the colors
			cur_fg = foreground;
			cur_bg = background;
		}
		else
		{
			// We're looking for the color!
			size_t comma = tag.find(",");
			if (comma == std::string::npos)
			{
				// No comma - just setting fg
				cur_fg = color_string(tag);
				if (cur_fg == c_null)
				{
					debugmsg("Malformed color tag: %s", tag.c_str());
					throw std::logic_error("Malformed color tag:");
				}
			}
			else
			{
				EColor new_fg = color_string(tag.substr(0, comma)),
						new_bg = color_string(tag.substr(comma + 1));
				if (new_fg == c_null && new_bg == c_null)
				{
					debugmsg("Malformed color tag: %s", tag.c_str());
					throw std::logic_error("Malformed color tag:");
				}
				if (new_fg != c_null)
					cur_fg = new_fg;
				if (new_bg != c_null)
					cur_bg = new_bg;
			} // if comma was found
		} // color needed to be found
	} // while (tag != std::string::npos)

	// There's a little string left over; push it into our vectors!
	result.push_back({ text, cur_fg, cur_bg });
	return result;
}

std::string strip_tags(std::string text);

Window::Window() : Window(0, 0, 0, 0)
{
	// Delegate the construction to another construct
}

Window::Window(int posx, int posy, int sizex, int sizey, TypeWindow ntype)
{
	render = std::make_unique<Editogia::Curses>(posx, posy, sizex, sizey);

	outlined = false;
	xdim = sizex;
	ydim = sizey;
	type = ntype;
}

void Window::outline()
{
	for (std::size_t startX = 0; startX < xdim; ++startX)
	{
		writeChar(startX, 0, 4194417);
		writeChar(startX, ydim - 1, 4194417);
	}

	for (std::size_t startY = 0; startY < ydim; ++startY)
	{
		writeChar(0, startY, 4194424);
		writeChar(xdim - 1, startY, 4194424);
	}

	writeChar(0, 0, 4194412);
	writeChar(0, ydim - 1, 4194413);
	writeChar(xdim - 1, 0, 4194411);
	writeChar(xdim - 1, ydim - 1, 4194410);
}

Glyph Window::glyphat(int x, int y)
{
	Glyph ret;
	if (type == TypeWindow::Text || type == TypeWindow::Other
		|| x < 0 || x >= xdim || y < 0 || y >= ydim)
	{
		return ret; // Whatever a default Glyph is
	}

	ret.symbol = render->getCharacterAt(x, y);
	ret.fg = render->getForegroundColorAt(x, y);
	ret.bg = render->getBackgroundColorAt(x, y);

	return ret;
}

void Window::putch(int x, int y, EColor fg, EColor bg, long sym)
{
	writeChar(x, y, sym, fg, bg);
}

void Window::putglyph(int x, int y, Glyph gl)
{
	if (type == TypeWindow::Text || type == TypeWindow::Other)
		return;
	putch(x, y, gl.fg, gl.bg, gl.symbol);
}

void Window::writeColorText(std::int32_t x, std::int32_t y, std::string_view text,
		const Window::Color foreground, const Window::Color background)
{
	if (text.find("<c=") == std::string::npos)
	{
		writeString(x, y, text, foreground, background);
	}
	else
	{
		std::vector<Editogia::TextColor> textColors = parseText(
				std::string(text), foreground, background);

		for (const auto textColor : textColors)
		{
			writeString(x, y, textColor.getText(), textColor.getForegroundColor(),
					textColor.getBackgroundColor());
			x += textColor.getText().size();
		}
	}
}

void Window::putstr(int x, int y, EColor fg, EColor bg, std::string str,
		...)
{
	if (type == TypeWindow::Glyphs)
		return;
	va_list ap;
	va_start(ap, str);
	char buff[8192];
	vsprintf(buff, str.c_str(), ap);
	va_end(ap);

	writeColorText(x, y, buff, fg, bg);
}

void Window::putstr_raw(int x, int y, EColor fg, EColor bg, std::string str,
		...)
{
	if (type == TypeWindow::Glyphs)
		return;
	va_list ap;
	va_start(ap, str);
	char buff[8192];
	vsprintf(buff, str.c_str(), ap);
	va_end(ap);

	writeString(x, y, buff, fg, bg);
}

void Window::putstr_n(int x, int y, EColor fg, EColor bg, int maxlength,
		std::string str, ...)
{
	if (type == TypeWindow::Glyphs)
		return;
	va_list ap;
	va_start(ap, str);
	char buff[8192];
	vsprintf(buff, str.c_str(), ap);
	va_end(ap);

	writeColorText(x, y, buff, fg, bg);
}

void Window::putstr_r(int x, int y, EColor fg, EColor bg, int maxlength,
		std::string str, ...)
{
	if (type == TypeWindow::Glyphs)
		return;
	va_list ap;
	va_start(ap, str);
	char buff[8192];
	vsprintf(buff, str.c_str(), ap);
	va_end(ap);

	std::string prepped = buff;
	std::string tagless = strip_tags(prepped);
	//std::string tagless = prepped;
	if (tagless.length() < maxlength)
	{
		x += maxlength - tagless.length();
	}

	writeColorText(x, y, prepped, fg, bg);
}

void Window::putstr_c(int x, int y, EColor fg, EColor bg, int maxlength,
		std::string str, ...)
{
	if (type == TypeWindow::Glyphs)
		return;
	va_list ap;
	va_start(ap, str);
	char buff[8192];
	vsprintf(buff, str.c_str(), ap);
	va_end(ap);

	std::string prepped = buff;
	std::string tagless = strip_tags(prepped);
	//std::string tagless = prepped;
	if (tagless.length() < maxlength)
	{
		x += (maxlength - tagless.length()) / 2;
	}

	writeColorText(x, y, prepped, fg, bg);
}

void Window::clear_area(int x1, int y1, int x2, int y2)
{
	if (type == TypeWindow::Other)
		return;
	for (int x = x1; x <= x2; x++)
	{
		for (int y = y1; y <= y2; y++)
			putch(x, y, c_black, c_black, 'x');
	}
}

void Window::line_v(int x, EColor fg, EColor bg)
{
	if (type != TypeWindow::General) // Only general (read: non-graphic) windows!
		return;
	for (int y = (outlined ? 1 : 0); y < (outlined ? ydim - 1 : ydim); y++)
		putch(x, y, fg, bg, LINE_XOXO);

	if (outlined)
	{ // Alter the outline so it attaches to our line
		putch(x, 0, fg, bg, LINE_OXXX);
		putch(x, ydim - 1, fg, bg, LINE_XXOX);
	}
}

void Window::line_h(int y, EColor fg, EColor bg)
{
	if (type != TypeWindow::General) // Only general (read: non-graphic) windows!
		return;
	for (int x = (outlined ? 1 : 0); x < (outlined ? xdim - 1 : xdim); x++)
		putch(x, y, fg, bg, LINE_OXOX);

	if (outlined)
	{ // Alter the outline so it attaches to our line
		putch(0, y, fg, bg, LINE_XXXO);
		putch(xdim - 1, y, fg, bg, LINE_XOXX);
	}
}

void Window::clear()
{
	render->clear();
}

void Window::refresh()
{
	render->draw();
}

const Editogia::Key Window::getKeyEvent()
{
	return render->getKeyEvent();
}

void Window::writeChar(const std::int32_t x, const std::int32_t y, const std::int32_t _char)
{
	writeChar(x, y, _char, c_white, c_black);
}

void Window::writeChar(const std::int32_t x, const std::int32_t y, const std::int32_t _char,
		const Window::Color foreground, const Window::Color background)
{
	render->writeChar(x, y, _char, foreground, background);
}

void Window::writeString(const std::int32_t x, const std::int32_t y, std::string_view _char)
{
	writeString(x, y, _char, c_white, c_black);
}

void Window::writeString(const std::int32_t x, const std::int32_t y, std::string_view _char,
		const Window::Color foreground, const Window::Color background)
{
	std::int32_t offsetX = x;

	for (const std::int32_t character : _char)
	{
		writeChar(offsetX, y, character, foreground, background);
		offsetX += 1;
	}
}

long input()
{
	flushinp();
	return getch();
}

void debugmsg(const char* mes, ...)
{
	va_list ap;
	va_start(ap, mes);
	char buff[2048];
	vsprintf(buff, mes, ap);
	va_end(ap);
	attron(COLOR_PAIR(3));
	mvprintw(0, 0, "DEBUG: %s      \n  Press spacebar...", buff);
	while (getch() != ' ');
	attroff(COLOR_PAIR(3));
}

void get_screen_dims(int& xdim, int& ydim)
{
	getmaxyx(stdscr, ydim, xdim);
}

std::string key_name(long ch)
{
	switch (ch)
	{
	case KEY_UP:
		return "UP";
	case KEY_RIGHT:
		return "RIGHT";
	case KEY_LEFT:
		return "LEFT";
	case KEY_DOWN:
		return "DOWN";
	case '\n':
		return "ENTER";
	case '\t':
		return "TAB";
	case KEY_ESC:
		return "ESC";
	case KEY_BACKSPACE:
	case 127:
	case 8:
		return "BACKSPACE";
	default:
		if (ch < 256)
		{
			std::stringstream ret;
			ret << char(ch);
			return ret.str();
		}
		else
		{
			std::stringstream ret;
			ret << "[" << int(ch) << "]";
			return ret.str();
		}
	}
	return "???";
}

bool is_backspace(long ch)
{
	return (ch == KEY_BACKSPACE || ch == 127 || ch == 8);
}

std::string string_input_popup(const char* mes, ...)
{
	va_list ap;
	va_start(ap, mes);
	char buff[1024];
	vsprintf(buff, mes, ap);
	va_end(ap);
	return string_edit_popup("", buff);
}

std::string string_edit_popup(std::string orig, const char* mes, ...)
{
	std::string ret = orig;
	va_list ap;
	va_start(ap, mes);
	char buff[1024];
	vsprintf(buff, mes, ap);
	va_end(ap);
	int startx = strlen(buff) + 2;
	Window w(0, 11, 80, 3);
	w.outline();
	w.putstr(1, 1, c_ltred, c_black, buff);
	w.putstr(startx, 1, c_magenta, c_black, ret);
	for (int i = startx + ret.length() + 1; i < 79; i++)
		w.putch(i, 1, c_ltgray, c_black, '_');
	int posx = startx + ret.length();
	w.putch(posx, 1, c_ltgray, c_blue, '_');
	do
	{
		w.refresh();
		long ch = getch();
		if (ch == 27)
		{    // Escape
			return orig;
		}
		else if (ch == '\n')
		{
			return ret;
		}
		else if (ch == KEY_BACKSPACE || ch == 127)
		{
			if (posx > startx)
			{
// Move the cursor back and re-draw it
				ret = ret.substr(0, ret.size() - 1);
				w.putch(posx, 1, c_ltgray, c_black, '_');
				posx--;
				w.putch(posx, 1, c_ltgray, c_blue, '_');
			}
		}
		else
		{
			ret += ch;
			w.putch(posx, 1, c_magenta, c_black, ch);
			posx++;
			w.putch(posx, 1, c_ltgray, c_blue, '_');
		}
	} while (true);
}

int int_input_popup(const char* mes, ...)
{
	std::string ret;
	bool negative = false;
	va_list ap;
	va_start(ap, mes);
	char buff[1024];
	vsprintf(buff, mes, ap);
	va_end(ap);
	int startx = strlen(buff) + 3;
	Window w(0, 11, 80, 3);
	w.outline();
	w.putstr(1, 1, c_ltred, c_black, buff);
	w.putstr(startx, 1, c_magenta, c_black, ret);
	for (int i = startx + ret.length() + 1; i < 79; i++)
		w.putch(i, 1, c_ltgray, c_black, '_');
	int posx = startx + ret.length();
	w.putch(posx, 1, c_ltgray, c_blue, '_');
	bool done = false;
	while (!done)
	{
		w.refresh();
		long ch = getch();
		if (ch == 27)
		{    // Escape
			return 0;
		}
		else if (ch == '\n')
		{
			done = true;
		}
		else if (ch == KEY_BACKSPACE || ch == 127)
		{
			if (posx > startx)
			{
// Move the cursor back and re-draw it
				ret = ret.substr(0, ret.size() - 1);
				w.putch(posx, 1, c_ltgray, c_black, '_');
				posx--;
				w.putch(posx, 1, c_ltgray, c_blue, '_');
			}
		}
		else if (ch >= '0' && ch <= '9')
		{
			ret += ch;
			w.putch(posx, 1, c_magenta, c_black, ch);
			posx++;
			w.putch(posx, 1, c_ltgray, c_blue, '_');
		}
		else if (ch == '-')
		{
			negative = !negative;
			if (negative)
				w.putch(startx - 1, 1, c_magenta, c_black, '-');
			else
				w.putch(startx - 1, 1, c_black, c_black, 'x');
		}
	}

	int retnum = 0;
	for (int i = 0; i < ret.length(); i++)
	{
		int val = ret[i] - '0';
		for (int n = 0; n < (ret.length() - 1 - i); n++)
			val *= 10;
		retnum += val;
	}
	if (negative)
		retnum *= -1;
	return retnum;
}

long popup_getkey(const char* mes, ...)
{
	va_list ap;
	va_start(ap, mes);
	char buff[8192];
	vsprintf(buff, mes, ap);
	va_end(ap);
	std::string tmp = buff;
	int width = 0;
	int height = 2;
	size_t pos = tmp.find('\n');
	while (pos != std::string::npos)
	{
		height++;
		if (pos > width)
			width = pos;
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	if (width == 0 || tmp.length() > width)
		width = tmp.length();
	width += 2;
	if (height > 25)
		height = 25;
	Window w(int((80 - width) / 2), int((25 - height) / 2), width, height + 1);
	w.outline();
	tmp = buff;
	pos = tmp.find('\n');
	int line_num = 0;
	while (pos != std::string::npos)
	{
		std::string line = tmp.substr(0, pos);
		line_num++;
		w.putstr(1, line_num, c_white, c_black, line);
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	line_num++;
	w.putstr(1, line_num, c_white, c_black, std::string(tmp));

	w.refresh();
	long ch = getch();
	return ch;
}

bool query_yn(const char* mes, ...)
{
	va_list ap;
	va_start(ap, mes);
	char buff[8192];
	vsprintf(buff, mes, ap);
	va_end(ap);
	std::string tmp = buff;
	int width = 0;
	int height = 2;
	size_t pos = tmp.find('\n');
	while (pos != std::string::npos)
	{
		height++;
		if (pos > width)
			width = pos;
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	if (width == 0 || tmp.length() > width)
		width = tmp.length();
	width += 2;
	if (height > 25)
		height = 25;
	Window w(int((80 - width) / 2), int((25 - height) / 2), width, height + 1);
	w.outline();
	tmp = buff;
	pos = tmp.find('\n');
	int line_num = 0;
	while (pos != std::string::npos)
	{
		std::string line = tmp.substr(0, pos);
		line_num++;
		w.putstr(1, line_num, c_white, c_black, line);
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	line_num++;
	w.putstr(1, line_num, c_white, c_black, std::string(tmp));

	w.refresh();
	return w.getKeyEvent().getKeyCode() == Editogia::KeyCode::Y;
}

int menu_vec(const char* mes, std::vector<std::string> options)
{
	if (options.size() == 0)
	{
		debugmsg("0-length menu (\"%s\")", mes);
		return -1;
	}
	std::string title = mes;
	int height = 3 + options.size(), width = title.length() + 2;
	for (int i = 0; i < options.size(); i++)
	{
		if (options[i].length() + 6 > width)
			width = options[i].length() + 6;
	}
	Window w(10, 6, width, height);
	w.outline();
	w.putstr(1, 1, c_white, c_black, title);

	for (int i = 0; i < options.size(); i++)
		w.putstr(1, i + 2, c_white, c_black, "%c: %s", 'a' + i, options[i].c_str());
	long ch;
	w.refresh();
	do
		ch = getch();
	while (ch < 'a' || ch >= 'a' + options.size());
	return (ch - 'a');
}

int menu(const char* mes, ...)
{
	va_list ap;
	va_start(ap, mes);
	char* tmp;
	std::vector<std::string> options;
	bool done = false;
	while (!done)
	{
		tmp = va_arg(ap, char*);
		if (tmp != NULL)
		{
			std::string strtmp = tmp;
			options.push_back(strtmp);
		}
		else
			done = true;
	}
	return (menu_vec(mes, options));
}

void popup(const char* mes, ...)
{
	va_list ap;
	va_start(ap, mes);
	char buff[8192];
	vsprintf(buff, mes, ap);
	va_end(ap);
	std::string tmp = buff;
	int width = 0;
	int height = 2;
	size_t pos = tmp.find('\n');
	while (pos != std::string::npos)
	{
		height++;
		if (pos > width)
			width = pos;
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	if (width == 0 || tmp.length() > width)
		width = tmp.length();
	width += 2;
	if (height > 25)
		height = 25;
	Window w(int((80 - width) / 2), int((25 - height) / 2), width, height + 1);
	w.outline();
	tmp = buff;
	pos = tmp.find('\n');
	int line_num = 0;
	while (pos != std::string::npos)
	{
		std::string line = tmp.substr(0, pos);
		line_num++;
		w.putstr(1, line_num, c_white, c_black, line.c_str());
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	line_num++;
	w.putstr(1, line_num, c_white, c_black, tmp.c_str());

	w.refresh();
	long ch;
	do
		ch = getch();
	while (ch != ' ' && ch != '\n' && ch != KEY_ESC);
}

void popup_nowait(const char* mes, ...)
{
	va_list ap;
	va_start(ap, mes);
	char buff[8192];
	vsprintf(buff, mes, ap);
	va_end(ap);
	std::string tmp = buff;
	int width = 0;
	int height = 2;
	size_t pos = tmp.find('\n');
	while (pos != std::string::npos)
	{
		height++;
		if (pos > width)
			width = pos;
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	if (width == 0 || tmp.length() > width)
		width = tmp.length();
	width += 2;
	if (height > 25)
		height = 25;
	Window w(int((80 - width) / 2), int((25 - height) / 2), width, height + 1);
	w.outline();
	tmp = buff;
	pos = tmp.find('\n');
	int line_num = 0;
	while (pos != std::string::npos)
	{
		std::string line = tmp.substr(0, pos);
		line_num++;
		w.putstr(1, line_num, c_white, c_black, line.c_str());
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	line_num++;
	w.putstr(1, line_num, c_white, c_black, tmp.c_str());

	w.refresh();
}

void popup_fullscreen(const char* mes, ...)
{
	va_list ap;
	va_start(ap, mes);
	char buff[8192];
	vsprintf(buff, mes, ap);
	va_end(ap);
	std::string tmp = buff;
	int width = 80;
	int height = 24;
	Window w(0, 0, width, height);
	tmp = buff;
	int pos = tmp.find('\n');
	int line_num = 0;
	while (pos != std::string::npos)
	{
		std::string line = tmp.substr(0, pos);
		line_num++;
		w.putstr(1, line_num, c_white, c_black, line.c_str());
		tmp = tmp.substr(pos + 1);
		pos = tmp.find('\n');
	}
	line_num++;
	w.putstr(1, line_num, c_white, c_black, tmp.c_str());

	w.refresh();
	long ch;
	do
		ch = getch();
	while (ch != ' ' && ch != '\n' && ch != KEY_ESC);
}

std::string strip_tags(std::string text)
{
	size_t tag;
	std::string ret;
	while ((tag = text.find("<c=")) != std::string::npos)
	{
		ret += text.substr(0, tag);
// Strip off everything up to and including "<c="
		text = text.substr(tag + 3);
// Find the end of the tag
		size_t tagend = text.find(">");
		if (tagend == std::string::npos)
		{
// Couldn't find the end of the tag, so just return
			ret += text;
			return ret;
		}
		text = text.substr(tagend + 1);
	}
	ret += text;
	return ret;
}
