// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_TEXTCOLOR_HPP
#define EDITOGIA_TEXTCOLOR_HPP

#include <string>
#include "Editogia/Render/View/color.h"

namespace Editogia
{

	class TextColor
	{

	public:

		/**
		 * Definition public of Color.
		 */
		using Color = EColor;

	private:

		std::string text;

		Color foreground;

		Color background;

	public:

		TextColor() noexcept = default;

		TextColor(const std::string& text, const Color foreground, const Color background) noexcept;

		// Getters

		const std::string& getText() const;

		const Color getForegroundColor() const;

		const Color getBackgroundColor() const;

	};

}

#endif //EDITOGIA_TEXTCOLOR_HPP
