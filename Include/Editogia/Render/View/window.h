#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Editogia/Render/View/Glyph.h"
#include "Editogia/Render/IRender.hpp"
#include <string_view>
#include <cstdint>
#include <memory>
#include <vector>
#include <list>

#if (defined _WIN32 || defined WINDOWS)
#include "catacurse.h"
#else

#include <curses.h>

#endif

//      LINE_NESW  - X for on, O for off
#define LINE_XOXO 4194424
#define LINE_OXOX 4194417
#define LINE_XXOO 4194413
#define LINE_OXXO 4194412
#define LINE_OOXX 4194411
#define LINE_XOOX 4194410
#define LINE_XXXO 4194420
#define LINE_XXOX 4194422
#define LINE_XOXX 4194421
#define LINE_OXXX 4194423
#define LINE_XXXX 4194414

#define KEY_ESC 27


enum class TypeWindow
{
	General,
	Glyphs,
	Text,
	Other
};

void init_display();

long input();

void debugmsg(const char* mes, ...);

void refresh_all(bool erase = false);

void get_screen_dims(int& xdim, int& ydim);

std::string key_name(long ch);

bool is_backspace(long ch);

std::string file_selector(std::string start = ".");

std::string string_edit_popup(std::string orig, const char* mes, ...);

std::string string_input_popup(const char* mes, ...);

int int_input_popup(const char* mes, ...);

long popup_getkey(const char* mes, ...);

bool query_yn(const char* mes, ...);

int menu_vec(const char* mes, std::vector<std::string> options);

int menu(const char* mes, ...);

void popup(const char* mes, ...);

void popup_nowait(const char* mes, ...);

void popup_fullscreen(const char* mes, ...);

void popup_scrollable(const char* mes, ...);

class Window
{
public:

	/**
	 * Definition public of Color.
	 */
	using Color = EColor;

	Window();

	Window(int posx, int posy, int sizex, int sizey,
			TypeWindow ntype = TypeWindow::General);

	~Window();

	void init(int posx, int posy, int sizex, int sizey,
			TypeWindow ntype = TypeWindow::General);

	void close();

	void outline();

// Info functions
	Glyph glyphat(int x, int y);

	int sizex()
	{
		return xdim;
	}

	int sizey()
	{
		return ydim;
	}

	void writeChar(const std::int32_t x, const std::int32_t y, const std::int32_t _char);

	void writeChar(const std::int32_t x, const std::int32_t y, const std::int32_t _char,
			const Color foreground, const Color background);

	void writeString(const std::int32_t x, const std::int32_t y, std::string_view _char);

	void writeString(const std::int32_t x, const std::int32_t y, std::string_view _char,
			const Color foreground, const Color background);

// The three essential output functions
	void putch(int x, int y, EColor fg, EColor bg, long sym);

	void putglyph(int x, int y, Glyph gl);

// Putstr places a string (unless we're designated as tiles-only)
	void putstr(int x, int y, EColor fg, EColor bg, std::string str, ...);

// Putstr_raw ignores color tags
	void putstr_raw(int x, int y, EColor fg, EColor bg, std::string str, ...);

// Putstr_n limits the length to maxlength
	void putstr_n(int x, int y, EColor fg, EColor bg, int maxlength,
			std::string str, ...);

// Putstr_r is like putstr_n, but right-aligned
	void putstr_r(int x, int y, EColor fg, EColor bg, int maxlength,
			std::string str, ...);

// Putstr_c is like putstr_n, but center-aligned
	void putstr_c(int x, int y, EColor fg, EColor bg, int maxlength,
			std::string str, ...);

// Special helper drawing functions
	void clear_area(int x1, int y1, int x2, int y2);

	void line_v(int x, EColor fg = c_white, EColor bg = c_black);

	void line_h(int y, EColor fg = c_white, EColor bg = c_black);

	void clear();

	void refresh();

private:
	WINDOW* w;
	bool outlined;
	TypeWindow type;
	int xdim, ydim;

	std::unique_ptr<Editogia::IRender> render{ nullptr };
};

extern std::list<Window*> WINDOWLIST;

#endif
