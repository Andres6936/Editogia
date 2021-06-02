// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_IRENDER_HPP
#define EDITOGIA_IRENDER_HPP

namespace Editogia
{

	class IRender
	{

	public:

		void writeChar(const std::int32_t x, const std::int32_t y, const std::int32_t _char,
				const Color foreground, const Color background);

		void writeString(const std::int32_t x, const std::int32_t y, std::string_view _char,
				const Color foreground, const Color background);

	};

}

#endif //EDITOGIA_IRENDER_HPP
