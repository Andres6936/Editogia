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

		/**
		 * Constructor for default
		 */
		Curses() noexcept;

		/**
		 * @param width Width in characters of window.
		 * @param height Height in characters of window.
		 */
		Curses(const std::int32_t width, const std::int32_t height) noexcept;

		/**
		 * @param x Coordinate at x of windows.
		 * @param y Coordinate at y of windows.
		 * @param width Width in characters of window.
		 * @param height Height in characters of window.
		 */
		Curses(const std::int32_t x, const std::int32_t y,
				const std::int32_t width, const std::int32_t height) noexcept;

		/**
		 * Destructor
		 */
		virtual ~Curses() noexcept;

		// Implemented abstract methods

		/**
		 * Draw th content of window.
		 */
		void draw() override;

		/**
		 * Clear the content of window.
		 */
		void clear() override;

		const std::int32_t getCharacterAt(const std::int32_t x, const std::int32_t y) override;

		const Color getForegroundColorAt(const std::int32_t x, const std::int32_t y) override;

		const Color getBackgroundColorAt(const std::int32_t x, const std::int32_t y) override;

		void writeChar(const std::int32_t x, const std::int32_t y, const std::int32_t _char,
				const Color foreground, const Color background) override;
	};

}

#endif //EDITOGIA_CURSES_HPP
