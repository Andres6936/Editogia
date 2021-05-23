// Joan Andrés (@Andres6936) Github.

#include <sstream>
#include "Editogia/Recipe.hpp"


// R defaults to RES_NULL, A defaults to 1
Recipe::Recipe(Resource R, int A)
{
	result = Resource_amount(R, A);
	units_per_day = 1;
	days_per_unit = 0;
	max_deficit = -10;
}

std::string Recipe::save_data()
{
	std::stringstream ret;

// Since some names are multi-word, use ! as a terminator
	ret << name << " ! ";
	ret << result.type << " " << result.amount << " ";
	ret << units_per_day << " ";
	ret << days_per_unit << " ";
	ret << std::endl;

	ret << resource_ingredients.size() << " ";
	for (int i = 0; i < resource_ingredients.size(); i++)
	{
		ret << resource_ingredients[i].type << " " <<
			resource_ingredients[i].amount << " ";
	}
	ret << std::endl;

	ret << mineral_ingredients.size() << " ";
	for (int i = 0; i < mineral_ingredients.size(); i++)
	{
		ret << mineral_ingredients[i].type << " " <<
			mineral_ingredients[i].amount << " ";
	}
	ret << std::endl;

	return ret.str();
}

bool Recipe::load_data(std::istream& data)
{
	name = "";

	std::string tmpstr;
	while (tmpstr != "!")
	{
		data >> tmpstr;
		if (tmpstr != "!")
		{
			if (!name.empty())
			{
				name = name + " ";
			}
			name = name + tmpstr;
		}
	}

	int tmptype, tmpnum;
	data >> tmptype >> tmpnum;
	if (tmptype <= 0 || tmptype >= RES_MAX)
	{
		debugmsg("Recipe loaded result of type %d (range is 1 to %d).",
				tmptype, RES_MAX - 1);
		return false;
	}
	result = Resource_amount(Resource(tmptype), tmpnum);

	data >> units_per_day >> days_per_unit;

	resource_ingredients.clear();
	int num_res;
	data >> num_res;
	for (int i = 0; i < num_res; i++)
	{
		data >> tmptype >> tmpnum;
		if (tmptype <= 0 || tmptype >= RES_MAX)
		{
			debugmsg("Recipe loaded res ingredient of type %d (range is 1 to %d).",
					tmptype, RES_MAX - 1);
			return false;
		}
		resource_ingredients.push_back(Resource_amount(Resource(tmptype), tmpnum));
	}

	mineral_ingredients.clear();
	int num_min;
	data >> num_min;
	for (int i = 0; i < num_min; i++)
	{
		data >> tmptype >> tmpnum;
		if (tmptype <= 0 || tmptype >= MINERAL_MAX)
		{
			debugmsg("Recipe loaded min ingredient of type %d (range is 1 to %d).",
					tmptype, MINERAL_MAX - 1);
			return false;
		}
		mineral_ingredients.push_back(Mineral_amount(Mineral(tmptype), tmpnum));
	}

	return true;
}

std::string Recipe::get_name()
{
	if (!name.empty())
	{
		return name;
	}

	Resource res = get_resource();
	return Resource_data[res]->name;
}

Resource Recipe::get_resource()
{
	return result.type;
}

