// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_CURSES_HPP
#define EDITOGIA_CURSES_HPP

#include <curses.h>
#include <utility>
#include "Editogia/IO/User/Key.hpp"
#include "Editogia/Render/IRender.hpp"

namespace Editogia
{

	class Curses : public IRender
	{

	private:

		/**
		 * Determine if the Backend (in this case NCurses has been initialized),
		 * the variable will be false while there aren't window created, with
		 * the first window created the backend will be initialized and this
		 * variable set to true.
		 */
		inline static bool startUp = false;

		/**
		 * Context ncurses.
		 */
		WINDOW* window{ nullptr };

		void startUpColorSchema();

		/**
		 * @param x Coordinate at x of cell.
		 * @param y Coordinate at y of cell.
		 * @return Tuple with the values of color and attributes in the cell.
		 */
		const std::pair<std::uint64_t, std::uint64_t>
		getColorAt(const std::int32_t x, const std::int32_t y);

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

		const Key getKeyEvent() override;

		const std::int32_t getCharacterAt(const std::int32_t x, const std::int32_t y) override;

		const Color getForegroundColorAt(const std::int32_t x, const std::int32_t y) override;

		const Color getBackgroundColorAt(const std::int32_t x, const std::int32_t y) override;

		void writeChar(const std::int32_t x, const std::int32_t y, const std::int32_t _char,
				const Color foreground, const Color background) override;
	};

}

#endif //EDITOGIA_CURSES_HPP
