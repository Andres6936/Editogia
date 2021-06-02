#include "Editogia/Entity/Org/ai.h"
#include "Editogia/Entity/Org/area.h"
#include "Editogia/Entity/animal.h"
#include "Editogia/Gea/map.h"
#include "Editogia/Gea/resource.h"
#include "Editogia/Gea/resource.h"
#include "Editogia/Mechanism/building.h"
#include "Editogia/Render/View/help.h"
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
