// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_TYPE_HPP
#define EDITOGIA_TYPE_HPP

enum element_type
{
	ELE_NULL = 0, // Nothing
	ELE_DRAWING,  // Plain characters / text
	ELE_TEXTBOX,  // Scrollable text
	ELE_LIST,     // Scrollable list w/ selection
	ELE_TEXTENTRY,// Type to enter text
	ELE_NUMBER,   // Number to select
	ELE_MENU,     // Drop-down menu
	ELE_MAX
};

std::string element_type_name(element_type type);

#endif //EDITOGIA_TYPE_HPP
