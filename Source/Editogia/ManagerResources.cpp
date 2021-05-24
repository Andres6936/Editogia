// Joan Andrés (@Andres6936) Github.

#include <Levin/Log.hpp>
#include "Editogia/ManagerResources.hpp"

using namespace Editogia;

ManagerResources::ManagerResources()
{
	insert({ Resource::RES_GOLD, 0 });
	insert({ Resource::RES_FOOD, 0 });
	insert({ Resource::RES_WOOD, 0 });
	insert({ Resource::RES_STONE, 0 });
	insert({ Resource::RES_TIN, 0 });
	insert({ Resource::RES_COPPER, 0 });
	insert({ Resource::RES_IRON, 0 });
	insert({ Resource::RES_FIBER, 0 });
	insert({ Resource::RES_FUR, 0 });
	insert({ Resource::RES_LEATHER, 0 });
	insert({ Resource::RES_UNICORN_HORN, 0 });
	insert({ Resource::RES_SALT, 0 });
	insert({ Resource::RES_PEPPER, 0 });
	insert({ Resource::RES_CINNAMON, 0 });
	insert({ Resource::RES_CUMIN, 0 });
	insert({ Resource::RES_PAPRIKA, 0 });
	insert({ Resource::RES_TOBACCO, 0 });
	insert({ Resource::RES_CANNABIS, 0 });
	insert({ Resource::RES_AMANITAS, 0 });
	insert({ Resource::RES_AYAHUASCA, 0 });
	insert({ Resource::RES_SPICEREED, 0 });
	insert({ Resource::RES_CLOTHING, 0 });
	insert({ Resource::RES_COUTURE, 0 });
	insert({ Resource::RES_FUR_COATS, 0 });
	insert({ Resource::RES_LEATHER_COATS, 0 });
	insert({ Resource::RES_WOOD_HOUSEWARES, 0 });
	insert({ Resource::RES_TIN_HOUSEWARES, 0 });
	insert({ Resource::RES_COPPER_HOUSEWARES, 0 });
	insert({ Resource::RES_IRON_HOUSEWARES, 0 });
	insert({ Resource::RES_FURNITURE, 0 });
	insert({ Resource::RES_JEWELRY, 0 });
	insert({ Resource::RES_BLANK_BOOK, 0 });
	insert({ Resource::RES_FARMING, 0 });
	insert({ Resource::RES_MINING, 0 });
	insert({ Resource::RES_HUNTING, 0 });
	insert({ Resource::RES_LOGGING, 0 });
}

std::uint32_t ManagerResources::getAt(Resource type) noexcept
{
	try
	{
		return at(type);
	}
	catch (std::out_of_range& exception)
	{
		Levin::Log::Warning("Attempting to access to resource type that does not exist.");
		return 0;
	}
}

void ManagerResources::setAt(const std::int32_t value, const Resource type) noexcept
{
	try
	{
		at(type) = value;
	}
	catch (std::out_of_range& exception)
	{
		Levin::Log::Warning("Attempting to access to resource type that does not exist.");
	}
}
