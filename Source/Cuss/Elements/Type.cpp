// Joan Andrés (@Andres6936) Github.

#include "Cuss/Elements/Type.hpp"


std::string cuss::element_type_name(cuss::element_type type)
{
	switch (type)
	{
	case ELE_NULL:
		return "NULL";
	case ELE_DRAWING:
		return "Drawing";
	case ELE_TEXTBOX:
		return "Text";
	case ELE_LIST:
		return "List selection";
	case ELE_TEXTENTRY:
		return "Text Entry";
	case ELE_NUMBER:
		return "Number";
	case ELE_MENU:
		return "Menu";
	default:
		return "Unknown";
	}
	return "What the heck";
}

