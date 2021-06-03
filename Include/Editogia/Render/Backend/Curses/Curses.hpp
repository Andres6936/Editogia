// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_CURSES_HPP
#define EDITOGIA_CURSES_HPP

#include <curses.h>
#include "Editogia/Render/IRender.hpp"

namespace Editogia
{

	class Curses : public IRender
	{

	private:

		/**
		 * Context ncurses.
		 */
		WINDOW* window{ nullptr };

	public:

		Curses() noexcept;

		Curses(const std::int32_t width, const std::int32_t height) noexcept;

		Curses(const std::int32_t x, const std::int32_t y,
				const std::int32_t width, const std::int32_t height) noexcept;

		virtual ~Curses() noexcept;

		void writeChar(const std::int32_t x, const std::int32_t y, const std::int32_t _char,
				const Color foreground, const Color background) override;
	};

}

#endif //EDITOGIA_CURSES_HPP
