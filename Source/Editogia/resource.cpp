#include "Editogia/resource.h"
#include "Editogia/stringfunc.h"
#include "Editogia/rng.h"

const char* resourceToString(const Resource resource) noexcept
{
	switch (resource)
	{

	case RES_NULL:
		return "Null";
	case RES_GOLD:
		return "Gold";
	case RES_FOOD:
		return "Food";
	case RES_WOOD:
		return "Wood";
	case RES_STONE:
		return "Stone";
	case RES_TIN:
		return "Tin";
	case RES_COPPER:
		return "Copper";
	case RES_IRON:
		return "Iron";
	case RES_FIBER:
		return "Fiber";
	case RES_FUR:
		return "Fur";
	case RES_LEATHER:
		return "Leather";
	case RES_UNICORN_HORN:
		return "UnicornHorn";
	case RES_SALT:
		return "Salt";
	case RES_PEPPER:
		return "Pepper";
	case RES_CINNAMON:
		return "Cinnamon";
	case RES_CUMIN:
		return "Cumin";
	case RES_PAPRIKA:
		return "Paprika";
	case RES_TOBACCO:
		return "Tobacco";
	case RES_CANNABIS:
		return "Cannabis";
	case RES_AMANITAS:
		return "Amanitas";
	case RES_AYAHUASCA:
		return "Ayahuasca";
	case RES_SPICEREED:
		return "Spicereed";
	case RES_CLOTHING:
		return "Clothing";
	case RES_COUTURE:
		return "Couture";
	case RES_FUR_COATS:
		return "Coats";
	case RES_LEATHER_COATS:
		return "LeatherCoats";
	case RES_WOOD_HOUSEWARES:
		return "WoodHousewares";
	case RES_TIN_HOUSEWARES:
		return "TinHousewares";
	case RES_COPPER_HOUSEWARES:
		return "CopperHousewares";
	case RES_IRON_HOUSEWARES:
		return "IronHousewares";
	case RES_FURNITURE:
		return "Furniture";
	case RES_JEWELRY:
		return "Jewelry";
	case RES_BLANK_BOOK:
		return "Book";
	case RES_FARMING:
		return "Farming";
	case RES_MINING:
		return "Mining";
	case RES_HUNTING:
		return "Hunting";
	case RES_LOGGING:
		return "Logging";
	case RES_MAX:
		return "Max";
	}
	return "Unknown";
}

bool Resource_amount::is_infinite()
{
	return (amount == INFINITE_RESOURCE);
}

bool Crop_amount::is_infinite()
{
	return (amount == INFINITE_RESOURCE);
}

bool Mineral_amount::is_infinite()
{
	return (amount == INFINITE_RESOURCE);
}

Mineral_amount Mineral_amount::make_small()
{
	Mineral_amount ret;
	ret.type = type;
	ret.amount = rng(amount * .1, amount * .2);
	return ret;
}

Mineral_amount Mineral_amount::randomize()
{
	Mineral_amount ret;
	ret.type = type;
	ret.amount = rng(amount * .8, amount * 1.2);
	return ret;
}

Resource lookup_resource(std::string name)
{
	name = no_caps(trim(name));
	for (int i = 0; i < RES_MAX; i++)
	{
		Resource ret = Resource(i);
		if (name == no_caps(Resource_data[ret]->name))
		{
			return ret;
		}
	}
	return RES_NULL;
}

Luxury_type lookup_luxury_type(std::string name)
{
	name = no_caps(trim(name));
	for (int i = 0; i < LUX_MAX; i++)
	{
		Luxury_type ret = Luxury_type(i);
		if (no_caps(luxury_type_name(ret)) == name)
		{
			return ret;
		}
	}
	return LUX_NULL;
}

std::string luxury_type_name(Luxury_type type)
{
	switch (type)
	{
	case LUX_NULL:
		return "NULL";
	case LUX_SPICE:
		return "spice";
	case LUX_SMOKABLE:
		return "smokable";
	case LUX_HALLUCINOGEN:
		return "hallucinogen";
	case LUX_COATS:
		return "warm coats";
	case LUX_HOUSEWARES:
		return "housewares";
	case LUX_MAX:
		return "BUG - LUX_MAX";
	default:
		return "BUG - Unnamed Luxury_type";
	}
	return "BUG - Escaped luxury_type_name() switch!";
}

Crop_type lookup_crop_type(std::string name)
{
	name = no_caps(trim(name));
	for (int i = 0; i < CROPTYPE_MAX; i++)
	{
		Crop_type ret = Crop_type(i);
		if (no_caps(crop_type_name(ret)) == name)
		{
			return ret;
		}
	}
	return CROPTYPE_NULL;
}

std::string crop_type_name(Crop_type type)
{
	switch (type)
	{
	case CROPTYPE_NULL:
		return "NULL";
	case CROPTYPE_FOOD:
		return "food";
	case CROPTYPE_SPICE:
		return "spice";
	case CROPTYPE_DRUG:
		return "drug";
	case CROPTYPE_POISON:
		return "poison";
	case CROPTYPE_FIBER:
		return "fiber";
	case CROPTYPE_OTHER:
		return "other";
	case CROPTYPE_MAX:
		return "BUG - CROPTYPE_MAX";
	default:
		return "Unnamed Crop_type";
	}
	return "BUG - Escaped crop_type_name() switch";
}

nc_color crop_type_color(Crop_type type)
{
	switch (type)
	{
	case CROPTYPE_NULL:
		return c_ltgray;
	case CROPTYPE_FOOD:
		return c_ltgreen;
	case CROPTYPE_SPICE:
		return c_yellow;
	case CROPTYPE_DRUG:
		return c_magenta;
	case CROPTYPE_POISON:
		return c_ltred;
	case CROPTYPE_FIBER:
		return c_ltblue;
	case CROPTYPE_OTHER:
		return c_brown;
	case CROPTYPE_MAX:
		return c_ltgray;
	default:
		return c_ltgray;
	}
	return c_ltgray;
}

Crop search_for_crop(std::string name)
{
	name = no_caps(trim(name));
	for (int i = 1; i < CROP_MAX; i++)
	{
		Crop ret = Crop(i);
		if (Crop_data[ret]->name.find(name) != std::string::npos)
		{
			return ret;
		}
	}
	return CROP_NULL;
}

Mineral search_for_mineral(std::string name)
{
	name = no_caps(trim(name));
	for (int i = 1; i < MINERAL_MAX; i++)
	{
		Mineral ret = Mineral(i);
		if (Mineral_data[ret]->name.find(name) != std::string::npos)
		{
			return ret;
		}
	}
	return MINERAL_NULL;
}

std::string mineral_amount_ranking(Mineral_amount min_amt)
{
	if (min_amt.amount == INFINITE_RESOURCE)
	{
		return "unlimited";
	}
	if (min_amt.type == MINERAL_NULL || min_amt.type == MINERAL_MAX)
	{
		return "";
	}

	if (min_amt.amount <= 100)
	{
		return "trace";
	}
	if (min_amt.amount <= 500)
	{
		return "a little";
	}
	if (min_amt.amount <= 2000)
	{
		return "some";
	}
	if (min_amt.amount <= 5000)
	{
		return "lots of";
	}
	if (min_amt.amount <= 8000)
	{
		return "tons of";
	}
	return "a huge amount of";
}

// "<return> trees"
std::string trees_amount_ranking(int wood)
{
	if (wood == 0)
	{
		return "no";
	}
	if (wood < 40)
	{
		return "one or two";
	}
	if (wood < 100)
	{
		return "very few";
	}
	if (wood < 300)
	{
		return "few";
	}
	if (wood < 1000)
	{
		return "many";
	}
	if (wood < 10000)
	{
		return "dense";
	}
	if (wood < 30000)
	{
		return "very dense";
	}
	return "overgrown with";
}
