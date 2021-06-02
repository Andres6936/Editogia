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

		virtual void writeChar(const std::int32_t x, const std::int32_t y,
				const std::int32_t _char,
				const Color foreground,
				const Color background) = 0;
	};

}

#endif //EDITOGIA_IRENDER_HPP
