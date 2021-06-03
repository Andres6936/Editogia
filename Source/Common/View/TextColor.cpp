// Joan Andrés (@Andres6936) Github.

#include "Editogia/Render/View/TextColor.hpp"

using namespace Editogia;

TextColor::TextColor(const std::string& _text, const Color _foreground,
		const Color _background) noexcept: text(_text),
										   foreground(_foreground), background(_background)
{
	// Assignment in situ of variables
}

// Getters

const std::string& TextColor::getText() const
{
	return text;
}

const TextColor::Color TextColor::getForegroundColor() const
{
	return foreground;
}

const TextColor::Color TextColor::getBackgroundColor() const
{
	return background;
}
