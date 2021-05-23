#include "Editogia/building.h"
#include "Editogia/area.h"
#include "Editogia/map.h"
#include "Editogia/resource.h"
#include "Editogia/race.h"
#include "Editogia/animal.h"
#include "Editogia/ai.h"
#include "Editogia/help.h"
#include "Editogia/globals.h"

bool init_all_data()
{
	init_building_data();
	init_area_data();
	init_terrain_data();
	init_map_type_data();
	init_resource_data();
	init_animal_data();
	init_races();
	init_city_roles();

	HELP = new Help_database;
	init_help();

	return true;
}
