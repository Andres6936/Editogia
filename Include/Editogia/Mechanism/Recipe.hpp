// Joan Andrés (@Andres6936) Github.

#ifndef EDITOGIA_RECIPE_HPP
#define EDITOGIA_RECIPE_HPP

#include <string>
#include <vector>

#include "Editogia/Gea/resource.h"

struct Recipe
{
	// R and A combine to form a Resource_amount.
	Recipe(Resource R = RES_NULL, int A = 1);

	std::string save_data();

	bool load_data(std::istream& data);

	std::string get_name(); // Returns name, or the result name if name is empty
	Resource get_resource();  // Returns result.type()

	std::string name;

	Resource_amount result;

	/* Obviously, only one of the following two should be set.  In
	 * building_data.cpp, we use macros that set one variable and set the other to
	 * 0.  When producing recipes, we check for whichever is not zero and use that.
	 * Also, both are multiplied by the number of workers; 2 units per day actually
	 * produces 6 if we have 3 workers, and 3 days per unit is actually 1 day per
	 * unit if we have 3 workers.
	 */
	int units_per_day;  // Defaults to 1
	int days_per_unit;  // Defaults to 0

	// max_deficit is used for AI and defaults to -10.  Basically, an AI_city won't
	// add this building if it has a deficit of more than this value of any of the
	// required resources or minerals.
	int max_deficit;

	std::vector<Resource_amount> resource_ingredients;
	std::vector<Mineral_amount> mineral_ingredients;

	const std::string toJSON() const noexcept;
};

#endif //EDITOGIA_RECIPE_HPP
