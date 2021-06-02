// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_IRENDER_HPP
#define EDITOGIA_IRENDER_HPP

namespace Editogia
{

	class IRender
	{

	public:

		/**
		 * Definition public of Color.
		 */
		using Color = nc_color;

		virtual void writeChar(const std::int32_t x, const std::int32_t y,
				const std::int32_t _char,
				const Color foreground,
				const Color background) = 0;

		virtual void writeString(const std::int32_t x, const std::int32_t y,
				std::string_view _char,
				const Color foreground,
				const Color background) = 0;

	};

}

#endif //EDITOGIA_IRENDER_HPP
