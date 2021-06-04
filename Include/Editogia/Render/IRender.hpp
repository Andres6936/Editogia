// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_IRENDER_HPP
#define EDITOGIA_IRENDER_HPP

#include "Editogia/Render/View/color.h"
#include <string_view>
#include <cstdint>

namespace Editogia
{

	class IRender
	{

	public:

		/**
		 * Definition public of Color.
		 */
		using Color = EColor;

		/**
		 * Constructor
		 */
		IRender() noexcept = default;

		/**
		 * Destructor
		 */
		virtual ~IRender() noexcept = default;

		// Abstract methods

		/**
		 * Draw the content in the render.
		 */
		virtual void draw() = 0;

		/**
		 * Clear the content in the render
		 */
		virtual void clear() = 0;

		virtual const std::int32_t getCharacterAt(const std::int32_t x,
				const std::int32_t y) = 0;

		virtual const Color getForegroundColorAt(const std::int32_t x,
				const std::int32_t y) = 0;

		virtual const Color getBackgroundColorAt(const std::int32_t x,
				const std::int32_t y) = 0;

		virtual void writeChar(const std::int32_t x, const std::int32_t y,
				const std::int32_t _char,
				const Color foreground,
				const Color background) = 0;
	};

}

#endif //EDITOGIA_IRENDER_HPP
