#include <sstream>
#include <vector>
#include <math.h> // for pow() and sqrt()
#include <fstream>
#include "Editogia/Math/rng.h"
#include "Editogia/IO/User/keys.h" // for input_direction()
#include "Editogia/Gea/world.h"
#include "Editogia/Render/View/window.h"
#include "Editogia/Entity/animal.h"
#include "Editogia/Entity/Org/ai_city.h"
#include "Editogia/globals.h"
#include "Editogia/Entity/Org/kingdom.h"  // To color map based on Kingdom.
#include "Editogia/Math/pathfind.h" // For road building, trade route finding, and more!
#include "Editogia/Math/geometry.h"
#include "Editogia/Util/stringfunc.h" // For capitalize()
#include "Cuss/Interface.hpp"

#include <Levin/Log.hpp>

Map_seen::Map_seen()
{
}

Map_seen::~Map_seen()
{
}

void Map_seen::init(int S)
{
	size = S;
	std::vector<bool> tmp;
	for (int i = 0; i < size; i++)
	{
		tmp.push_back(false);
	}
	for (int i = 0; i < size; i++)
	{
		seen.push_back(tmp);
	}
}

std::string Map_seen::save_data()
{
	std::stringstream ret;

	ret << size << std::endl;

	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			ret << seen[x][y] << " ";
		}
	}
	return ret.str();
}

bool Map_seen::load_data(std::istream& data)
{
	data >> size;
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			int tmp_seen;
			data >> tmp_seen;
			seen[x][y] = tmp_seen;
		}
	}
	return true;
}

bool Map_seen::OOB(int x, int y)
{
	return (x < 0 || x >= size ||
			y < 0 || y >= size);
}

bool Map_seen::is_seen(Point p)
{
	return is_seen(p.x, p.y);
}

bool Map_seen::is_seen(int x, int y)
{
	if (OOB(x, y))
	{
		return false;
	}
	return seen[x][y];
}

void Map_seen::mark_seen(int x, int y)
{
	if (!OOB(x, y))
	{
		seen[x][y] = true;
	}
}

World_design::World_design()
{
	size = WORLD_SIZE_MEDIUM;
	temperature = WORLD_TEMP_TEMPERATE;
	rainfall = WORLD_RAIN_MODERATE;
	mountain = WORLD_MOUNTAIN_VARIED;
}

World_design::~World_design()
{
}



bool World::generate(World_design design)
{
// Some sanity checks
	if (design.size == WORLD_SIZE_MAX)
	{
		Levin::Log::Informational("World generated with size of WORLD_SIZE_MAX!");
		return false;
	}
	if (design.temperature == WORLD_TEMP_MAX)
	{
		Levin::Log::Informational("World generated with temperature of WORLD_TEMP_MAX!");
		return false;
	}
	if (design.rainfall == WORLD_RAIN_MAX)
	{
		Levin::Log::Informational("World generated with rainfall of WORLD_RAIN_MAX!");
		return false;
	}
	if (design.mountain == WORLD_MOUNTAIN_MAX)
	{
		Levin::Log::Informational("World generated with mountain of WORLD_MOUNTAIN_MAX!");
		return false;
	}

	if (!set_size(design.size))
	{
		return false;
	}

	name = design.name;

	int equator_temp = 100;
	switch (design.temperature)
	{
	case WORLD_TEMP_ICE_AGE:
		equator_temp = 25;
		break;
	case WORLD_TEMP_COLD:
		equator_temp = 50;
		break;
	case WORLD_TEMP_TEMPERATE:
		equator_temp = 100;
		break;
	case WORLD_TEMP_HOT:
		equator_temp = 120;
		break;
	case WORLD_TEMP_FURNACE:
		equator_temp = 175;
		break;
	}

	int temp_shift = 0;
	for (int x = 0; x < size; x++)
	{
		if (!one_in(3))
		{
			temp_shift += rng(-1, 1);
		}
		if (temp_shift < -15)
		{
			temp_shift = -15;
		}
		else if (temp_shift > 15)
		{
			temp_shift = 15;
		}
		for (int y = 0; y < size; y++)
		{
			tiles[x][y] = MAP_OCEAN;
			altitude[x][y] = -1;
			rainfall[x][y] = -1;
			if (y < size / 2)
			{
				temperature[x][y] = (equator_temp * (temp_shift + y)) / (size / 2);
			}
			else
			{
				int fy = size - 1 - y;
				temperature[x][y] = (equator_temp * (temp_shift + fy)) / (size / 2);
			}
			//temperature[x][y] += rng(-1, 1);
			//temperature[x][y] += rng(-3, 3);
			continent_id[x][y] = -1;
			kingdom_id[x][y] = -1;
			city[x][y] = NULL;
			river[x][y] = false;
			road[x][y] = false;
		}
	}

	continents.clear();

// Pick a bunch of points to base continents off of.
	int num_continents = size / rng(6, 10);
	for (int i = 0; i < num_continents; i++)
	{
		Point continent(rng(20, size - 21), rng(20, size - 21));
		continents.push_back(continent);
	}

// Build continent, and rivers there

/* We want to figure out the min/max size for continents, given the size of the
 * world map.  But continents are built by starting with a seed altitude, and
 * stepping down by some amount each tile (usually).  So the size (usually) is
 * about equal to the seed altitude divided by that step.  We don't want to
 * change our seed altitude, so we change the step instead.  csize_min/max is
 * the ratio of heigh-to-step.
 */
	int csize_min = size / 50, csize_max = size / 10;
	for (int i = 0; i < continents.size(); i++)
	{
		int height;
		int step; // Alt decreases by this each tile (usually).
		if (one_in(8))
		{ // Island
			if (one_in(2))
			{  // Low-altitude island
				height = rng(20, 50);
				step = rng(16 / csize_min, 240 / csize_max);
			}
			else
			{  // Mountainous island
				height = rng(100, 150);
				step = rng(200 / csize_min, 1400 / csize_max);
			}
		}
		else
		{  // Normal continent!
			height = rng(70, 125);
			step = rng(24 / csize_min, 320 / csize_max);
		}
		if (step > height / 3)
		{
			step = height / 3; // Let's be reasonable.
		}
		add_continent(continents[i], height, step, i);
	} // for (int i = 0; i < continents.size(); i++)

// Add some mountain ranges.
	int num_ranges = 0, range_width = 0;
	int alt_foothill = 55, alt_mountain = 80;
	switch (design.mountain)
	{
	case WORLD_MOUNTAIN_FLAT:
		num_ranges = 0;
		range_width = 0;
		alt_foothill = 80;
		alt_mountain = 100;
		break;
	case WORLD_MOUNTAIN_LOW:
		num_ranges = 2;
		range_width = 1;
		alt_foothill = 75;
		alt_mountain = 95;
		break;
	case WORLD_MOUNTAIN_VARIED:
		num_ranges = 4;
		range_width = 1;
		alt_foothill = 55;
		alt_mountain = 80;
		break;
	case WORLD_MOUNTAIN_HIGH:
		num_ranges = 8;
		range_width = 2;
		alt_foothill = 30;
		alt_mountain = 65;
		break;
	case WORLD_MOUNTAIN_COVERED:
		num_ranges = 15;
		range_width = 4;
		alt_foothill = 14;
		alt_mountain = 40;
		break;
	}

	int range_length_max = size / 3, range_length_min = size / 8;

	for (int i = 0; i < num_ranges; i++)
	{
// Pick a starting point - anywhere in the world (but far enough from the edges
// to guarantee an in-bound end point).
		Point start(rng(range_length_max, size - 1 - range_length_max),
				rng(range_length_max, size - 1 - range_length_max));

// Pick an ending point - fairly close to the starting point.
		int min_x, max_x, min_y, max_y;
		if (one_in(2))
		{  // Go west
			min_x = start.x - range_length_max;
			max_x = start.x - range_length_min;
		}
		else
		{  // Go east
			min_x = start.x + range_length_min;
			max_x = start.x + range_length_max;
		}
		if (one_in(2))
		{ // Go north
			min_y = start.y - range_length_max;
			max_y = start.y - range_length_min;
		}
		else
		{  // Go south
			min_y = start.y + range_length_max;
			max_y = start.y + range_length_min;
		}
		Point end(rng(min_x, max_x), rng(min_y, max_y));

// Generate a line between the two points.
		std::vector<Point> range_line = line_to(start, end);
// Iterate over the line.
		for (int n = 0; n < range_line.size(); n++)
		{
			Point p = range_line[n];
// Affect all tiles within range_width.  If we're close to the start or end of
// the range, use a shorter range_width and lower mountains.
			int range_dist = range_width;
			int alt_min = alt_foothill, alt_max = alt_mountain * 2;
			if (rl_dist(start, p) <= 3 || rl_dist(end, p) <= 3)
			{
				range_dist = 1;
				alt_min = alt_min / 2;
				alt_max = alt_mountain;
			}
			else if (rl_dist(start, p) <= 6 || rl_dist(end, p) <= 6)
			{
				alt_max = alt_mountain;
			}

			for (int x = p.x - range_dist; x <= p.x + range_dist; x++)
			{
				for (int y = p.y - range_dist; y <= p.y + range_dist; y++)
				{
					if (!OOB(x, y) && altitude[x][y] > 0)
					{
						altitude[x][y] = rng(alt_min, alt_max);
					}
				}
			}
		}
	} // for (int i = 0; i < num_ranges; i++)

// All continents get some rivers!
	for (int i = 0; i < continents.size(); i++)
	{
		Point river(continents[i].x + rng(-5, 5), continents[i].y + rng(-5, 5));
		add_river(river);
		int num = 3;
		while (one_in(num))
		{
			river.x += rng(-4, 4);
			river.y += rng(-4, 4);
			add_river(river);
			num++;
		}
	}

/* Set rainfall!  Start at the western edge of the map, then proceed eastward.
 * TODO:  It'd be cool if the northern hemisphere used east-blowing winds (as
 *        below), but the southern hemisphere used west-blowing winds.  And if
 *        wind strength were low at the equator and poles, and high in between.
 */

	int base_rainfall = 35, rain_shift_down = -2, rain_shift_up = 4,
			river_rain_min = 1, river_rain_max = 15, ocean_rain_chance = 4;
	switch (design.rainfall)
	{
	case WORLD_RAIN_DRY:
		base_rainfall = 0;
		rain_shift_down = -5;
		rain_shift_up = 1;
		river_rain_min = 0;
		river_rain_max = 4;
		ocean_rain_chance = 10;
		break;
	case WORLD_RAIN_LIGHT:
		base_rainfall = 15;
		rain_shift_down = -3;
		rain_shift_up = 2;
		river_rain_min = 1;
		river_rain_max = 8;
		ocean_rain_chance = 7;
		break;
	case WORLD_RAIN_MODERATE:
		base_rainfall = 35;
		rain_shift_down = -2;
		rain_shift_up = 4;
		river_rain_min = 1;
		river_rain_max = 15;
		ocean_rain_chance = 4;
		break;
	case WORLD_RAIN_HEAVY:
		base_rainfall = 50;
		rain_shift_down = -2;
		rain_shift_up = 5;
		river_rain_min = 2;
		river_rain_max = 20;
		ocean_rain_chance = 3;
		break;
	case WORLD_RAIN_UNENDING:
		base_rainfall = 100;
		rain_shift_down = -1;
		rain_shift_up = 8;
		river_rain_min = 5;
		river_rain_max = 30;
		ocean_rain_chance = 2;
		break;
	}
	for (int y = 0; y < size; y++)
	{
		rainfall[0][y] = base_rainfall;
	}
	for (int x = 1; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
// Copy the value of rainfall from the tile to the left (doubled to weight it).
			int past = rainfall[x - 1][y] * 2;
// Sources is the number of inputs we've used - later on we'll divide by this
// number to get an average.  Since inputs are weighted, sources starts as 2.
			int sources = 2;

// Increase rainfall if we're over the ocean.
			if (altitude[x][y] <= 0)
			{
				past += rng(0, 1);
				if (one_in(ocean_rain_chance))
				{
					past += rng(1, 2);  // Sometimes it increases by a lot!
				}
// If we're not over the ocean, occasionally decrease rainfall.
			}
			else if (one_in(15))
			{
				past -= rng(0, 1);
			}
// Use the tile to the northwest as a source - weighted higher!
			if (y > 0 && rainfall[x - 1][y - 1] > 0)
			{
				sources += 5;
				past += rainfall[x - 1][y - 1] * 5;
				if (rainfall[x - 1][y - 1] > rainfall[x - 1][y])
				{
					sources += 1;
					past += rainfall[x - 1][y - 1];
				}
			}
// Ditto for the tile to the southwest.
			if (y < size - 1 && rainfall[x - 1][y + 1] > 0)
			{
				sources += 5;
				past += rainfall[x - 1][y + 1] * 5;
				if (rainfall[x - 1][y + 1] > rainfall[x - 1][y])
				{
					sources += 1;
					past += rainfall[x - 1][y + 1];
				}
			}
			past /= sources;  // Grab the average.

// Lose a lot of rainfall if a mountain is blocking these moisture-laden winds.
			if (altitude[x][y] >= 85)
			{
				past = rng(0, past / 2);

// High hills sometimes block some moisture too.
			}
			else if (altitude[x][y] >= 60 && one_in(6))
			{
				past -= rng(0, 4);

// Low altitudes (and low rainfall) give us a chance to alter the rainfall -
// usually increasing it a bit.
			}
			else if (altitude[x][y] < 85 - past && one_in(3))
			{
				past = rng(past + rain_shift_down, past + rain_shift_up);
			}

// Rivers add rainfall, resulting in forests and swamps to the east.
			if (river[x][y])
			{
				if (one_in(10))
				{
					past += rng(river_rain_min * 10, river_rain_max * 3);
				}
				else
				{
					past += rng(river_rain_min, river_rain_max);
				}
			}

// High temperatures tend to decrease rainfall.
			if (past > temperature[x][y])
			{
				past = rng(temperature[x][y], past);
			}

// Finally, copy it over!
			rainfall[x][y] = past;
			if (rainfall[x][y] < 0)
			{
				rainfall[x][y] = 0;
			}
		}
	}

// Finally, set terrain based on altitude, temperature, rainfall and river
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{

// First category: high-altitude stuff.
			if (altitude[x][y] >= alt_mountain)
			{
				if (river[x][y])
				{
					if (temperature[x][y] <= 20)
					{
						tiles[x][y] = MAP_GLACIER;
					}
					else
					{
						tiles[x][y] = MAP_CANYON;
					}
				}
				else if (temperature[x][y] <= 20)
				{
					tiles[x][y] = MAP_ICY_MOUNTAIN;
				}
				else
				{
					tiles[x][y] = MAP_MOUNTAINOUS;
				}

// Next category: foothills.
			}
			else if (altitude[x][y] >= alt_foothill)
			{

// What kind of river is it?  We MIGHT have a canyon for high altitude.
				if (river[x][y])
				{
					if (temperature[x][y] <= 20)
					{
						tiles[x][y] = MAP_GLACIER;
					}
					else if (altitude[x][y] >= rng(60, 85))
					{
						tiles[x][y] = MAP_CANYON;
					}
					else
					{
						tiles[x][y] = MAP_BASIN;
					}

// High rainfall will override the hilly altitude and create jungle/forest.
				}
				else if (rainfall[x][y] - 55 > altitude[x][y] - alt_foothill)
				{
					if (temperature[x][y] > altitude[x][y] + 80 - alt_foothill)
					{
						tiles[x][y] = MAP_JUNGLE;
					}
					else
					{
						tiles[x][y] = MAP_FOREST;
					}

// Otherwise, we're just some kind of foothills.
				}
				else if (temperature[x][y] <= 20)
				{
					tiles[x][y] = MAP_ICY_FOOTHILLS;
				}
				else
				{
					tiles[x][y] = MAP_FOOTHILLS;
				}

// Next category: the ocean (or frozen ocean (aka icecap)).
			}
			else if (altitude[x][y] <= 0)
			{
				if (altitude[x][y] == 0 && temperature[x][y] <= 12)
				{
					tiles[x][y] = MAP_ICECAP;
				}
				else
				{
					tiles[x][y] = MAP_OCEAN;
				}

// Finally, all other altitudes are considered "flat."
			}
			else
			{
// Rivers are glaciers if it's cold, swamps if it's very rainy, or just rivers
				if (river[x][y])
				{
					if (temperature[x][y] <= 20)
					{
						tiles[x][y] = MAP_GLACIER;
					}
					else if (rainfall[x][y] >= 50)
					{  // "50" was "55", shld we revert?
						tiles[x][y] = MAP_SWAMP;
					}
					else
					{
						tiles[x][y] = MAP_BASIN;
					}
// Cold temps are always tundra
				}
				else if (temperature[x][y] <= 20)
				{
					tiles[x][y] = MAP_TUNDRA;
// High rainfall is always swamp
				}
				else if (rainfall[x][y] >= 55)
				{
					tiles[x][y] = MAP_SWAMP;
// Medium rainfall is forest or jungle, depending on temp
				}
				else if (rainfall[x][y] >= 45)
				{
					if (temperature[x][y] >= 75)
					{
						tiles[x][y] = MAP_JUNGLE;
					}
					else
					{
						tiles[x][y] = MAP_FOREST;
					}
// Medium-low rainfall is plains
				}
				else if (rainfall[x][y] >= 30)
				{
					tiles[x][y] = MAP_PLAINS;
// Low rainfall is wasteland
				}
				else if (rainfall[x][y] >= 12)
				{
					tiles[x][y] = MAP_WASTELAND;
// Very low rainfall is wasteland if cold, desert if warm
				}
				else if (temperature[x][y] <= 32)
				{
					tiles[x][y] = MAP_WASTELAND;
				}
				else
				{
					tiles[x][y] = MAP_DESERT;
				}
			}
		}
	}

// ... aaaand set up crops/minerals.
// Crops
	for (int i = 1; i < CROP_MAX; i++)
	{
		Crop crop = Crop(i);
		Crop_datum* crop_dat = Crop_data[crop];
		int min_radius = 5 + (crop_dat->percentage / 6);
		int max_radius = 10 + (crop_dat->percentage / 2);
		int avg_radius = (min_radius + max_radius) / 2;
		int avg_size = avg_radius * avg_radius;
// Calculate the total number of blobs of the given size that'd fit in the world
		int total_blobs = size * size;
		total_blobs /= avg_size;
		if (crop_dat->percentage < 20)
		{
			total_blobs *= .6;
		}
		else if (crop_dat->percentage > 90)
		{
			total_blobs *= 1.2;
		}
// Place an appropriate percentage of the total blobs
		int num_blobs = total_blobs * crop_dat->percentage;
		num_blobs /= 100;
		if (crop_dat->percentage >= 90)
		{
			max_radius += 8;
		}
// Now place the blobs.
		for (int n = 0; n < num_blobs; n++)
		{
			int percent = (100 * n) / num_blobs;
			if (percent >= 10 && percent % 5 == 0)
			{
				popup_nowait("\
Generating World...\n\
Crop %d / %d (%s)  \n\
Placing %d blobs [%d%%%%%%%%]",
						i, CROP_MAX - 1, Crop_data[crop]->name.c_str(),
						num_blobs, (100 * n) / num_blobs);
			}
			int radius = rng(min_radius, max_radius);
			Point p;
			int tries = 0;
			do
			{
				tries++;
				p = Point(rng(0, size - 1), rng(0, size - 1));
			} while (tries < 20 && !tile_okay_for_crop(p, crop));
			add_crop(p, crop, radius);
		}
	}

// Exact same thing, but for minerals.
	for (int i = 1; i < MINERAL_MAX; i++)
	{
		Mineral mineral = Mineral(i);
		Mineral_datum* mineral_dat = Mineral_data[mineral];
		int min_radius = 5 + (mineral_dat->percentage / 6);
		int max_radius = 10 + (mineral_dat->percentage / 2);
		int avg_radius = (min_radius + max_radius) / 2;
		int avg_size = avg_radius * avg_radius;
// Calculate the total number of blobs of the given size that'd fit in the world
		int total_blobs = size * size;
		total_blobs /= avg_size * 1.2;
		if (mineral_dat->percentage < 20)
		{
			total_blobs *= .6;
		}
		else if (mineral_dat->percentage > 90)
		{
			total_blobs *= 1.2;
		}
// Place an appropriate percentage of the total blobs
		int num_blobs = total_blobs * mineral_dat->percentage;
		num_blobs /= 100;
		if (mineral_dat->percentage >= 90)
		{
			max_radius += 8;
		}
// Now place the blobs.
		for (int n = 0; n < num_blobs; n++)
		{
			int percent = (100 * n) / num_blobs;
			if (percent >= 10 && percent % 5 == 0)
			{
				popup_nowait("\
Generating World...\n\
Minerals: %d / %d (%s)  \n\
Placing %d blobs [%d%%%%%%%%]",
						i, MINERAL_MAX - 1, Mineral_data[mineral]->name.c_str(),
						num_blobs, (100 * n) / num_blobs);
			}
			int radius = rng(min_radius, max_radius);
			Point p(rng(0, size - 1), rng(0, size - 1));
			add_mineral(p, mineral, radius);
		}
	}

// Slightly different for animals, but still very similar
	for (int i = 1; i < ANIMAL_MAX; i++)
	{
		Animal animal = Animal(i);
		Animal_datum* animal_dat = Animal_data[animal];

		int num_blobs = 5 + animal_dat->percentage / 2;

// We want those blobs to cover <animal_dat->percentage> percent of the world.
		if (num_blobs < 2)
		{
			num_blobs = 2;
		}
// So figure out a size that will do that.
		int needed_coverage = (animal_dat->percentage * size *
				size) / 100;
		int needed_area = needed_coverage / num_blobs;
		int needed_radius = sqrt(double(needed_area));
		int min_radius = needed_radius * 0.6;
		int max_radius = needed_radius * 1.4;
// Now place the blobs.
		for (int n = 0; n < num_blobs; n++)
		{
			int percent = (100 * n) / num_blobs;
			if (percent >= 10 && percent % 5 == 0)
			{
				popup_nowait("\
Generating World...\n\
Animal %d / %d (%s)  \n\
Placing %d blobs [%d%%%%%%%%]",
						i, ANIMAL_MAX - 1, Animal_data[animal]->name.c_str(),
						num_blobs, (100 * n) / num_blobs);
			}
			int radius = rng(min_radius, max_radius);
			Point p;
			int tries = 0;
			do
			{
				tries++;
				p = Point(rng(0, size - 1), rng(0, size - 1));
			} while (tries < 20 && !tile_okay_for_animal(p, animal));
			add_animal(p, animal, radius);
		}
	}

// Fix clustered rivers
/*
  std::vector<Point> swamps;
  for (int x = 0; x < size - 2; x++) {
    for (int y = 0; y < size - 2; y++) {
      int count = 0;
      for (int rx = x; rx <= x + 10; rx++) {
        for (int ry = y; ry <= y + 10; ry++) {
          if (tiles[rx][ry] == MAP_BASIN) {
            count++;
          }
        }
      }
      if (count >= 35) {
        for (int rx = x; rx <= x + 10; rx++) {
          for (int ry = y; ry <= y + 10; ry++) {
            if (tiles[rx][ry] == MAP_BASIN || one_in(6)) {
              swamps.push_back( Point(rx, ry) );
            }
          }
        }
      }
    }
  }
  for (int i = 0; i < swamps.size(); i++) {
    tiles[swamps[i].x][swamps[i].y] = MAP_SWAMP;
  }
*/

// Finally, generate our road map and travel maps.
	update_road_map();
	update_travel_map();  // By not passing a parameter, we update ALL races' maps

// Set the name.
/*
  Window w_name(24, 9, 32, 6);
  cuss::interface i_name;
  if (!i_name.load_from_file("cuss/set_world_name.cuss")) {
    set_random_name();
    debugmsg("Name set to '%s'.", name.c_str());
    return false;
  }

  i_name.select("entry_name");
  i_name.ref_data("entry_name", &name);
  bool done = false;

  while (!done) {
    i_name.draw(&w_name);
    w_name.refresh();

    long ch = input();

    if (ch == '!') {
      set_random_name();

    } else if (ch == '\n') {
      done = true;

    } else {
      i_name.handle_keypress(ch);
    }
  }
*/

	return true;
}

bool World::set_size(World_size Size)
{
	if (size <= 0)
	{
		debugmsg("World::set_size(%d) called!", Size);
		return false;
	}

	size = world_size_size(Size);

// Prep all our data vectors.
	std::vector<Map_type> tmp_map_type;
	std::vector<City*> tmp_ptr;
	std::vector<bool> tmp_bool;
	std::vector<int> tmp_int;
	for (int i = 0; i < size; i++)
	{
		tmp_map_type.push_back(MAP_NULL);
		tmp_ptr.push_back(NULL);
		tmp_bool.push_back(false);
		tmp_int.push_back(0);
	}
	tiles.clear();
	city.clear();
	river.clear();
	road.clear();
	altitude.clear();
	rainfall.clear();
	temperature.clear();
	continent_id.clear();
	kingdom_id.clear();
	crops.clear();
	minerals.clear();
	animals.clear();
	for (int i = 0; i < size; i++)
	{
		tiles.push_back(tmp_map_type);
		city.push_back(tmp_ptr);
		river.push_back(tmp_bool);
		road.push_back(tmp_bool);
		altitude.push_back(tmp_int);
		rainfall.push_back(tmp_int);
		temperature.push_back(tmp_int);
		continent_id.push_back(tmp_int);
		kingdom_id.push_back(tmp_int);
		crops.push_back(tmp_int);
		minerals.push_back(tmp_int);
		animals.push_back(tmp_int);
	}

	return true;
}

bool World::save_to_file(std::string filename)
{
	if (filename.empty())
	{
		return false;
	}
	std::ofstream fout;
	fout.open(filename.c_str());
	if (!fout.is_open())
	{
		return false;
	}
	fout << name << std::endl;
	fout << size << " " << continents.size() << " ";
	for (int i = 0; i < continents.size(); i++)
	{
		fout << continents[i].x << " " << continents[i].y << " ";
	}
	fout << std::endl;
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			fout << tiles[x][y] << " " <<
									   continent_id[x][y] << " " << // Not sure if we really need this
									   kingdom_id[x][y] << " " <<
									   road[x][y] << " " <<
									   crops[x][y] << " " <<
									   minerals[x][y] << " " <<
									   animals[x][y] << " ";
		}
		fout << std::endl;
	}

	fout << road_map.save_data() << std::endl;
// Start at 1 cause we don't do RACE_NULL
	for (int i = 1; i < RACE_MAX; i++)
	{
		fout << travel_map[Race(i)].save_data() << std::endl;
	}

	fout.close();
	return true;
}

bool World::load_from_file(std::string filename)
{
	if (filename.empty())
	{
		Levin::Log::Informational("World attempted to load an empty filename.");
		return false;
	}
	std::ifstream fin;
	fin.open(filename.c_str());
	if (!fin.is_open())
	{
		debugmsg("World couldn't open '%s' for loading.", filename.c_str());
		return false;
	}
	std::getline(fin, name);
	int tmp_size, num_continents;
	fin >> tmp_size >> num_continents;
	set_size(World_size(tmp_size));
	for (int i = 0; i < num_continents; i++)
	{
		Point p;
		fin >> p.x >> p.y;
		continents.push_back(p);
	}
	int tmp_map_type;
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			bool tmp_road;
			fin >> tmp_map_type >>
								continent_id[x][y] >>
								kingdom_id[x][y] >>
								tmp_road >>
								crops[x][y] >>
								minerals[x][y] >>
								animals[x][y];
			tiles[x][y] = Map_type(tmp_map_type);
			road[x][y] = tmp_road;
		}
	}

	if (!road_map.load_data(fin))
	{
		Levin::Log::Informational("World failed to load road_map.");
		return false;
	}

// Start at 1 cause we don't do RACE_NULL
	for (int i = 1; i < RACE_MAX; i++)
	{
		Generic_map tmpmap;
		if (!tmpmap.load_data(fin))
		{
			debugmsg("World failed to load travel_map for %s.",
					Race_data[i]->name.c_str());
			return false;
		}
		travel_map[Race(i)] = tmpmap;
	}
	fin.close();
	return true;
}

void World::add_continent(Point origin, int height, int step, int id)
{
	std::vector<Point> active;

	active.push_back(origin);

	altitude[origin.x][origin.y] = height;
	continent_id[origin.x][origin.y] = id;

/* Continent-building algorithm:
 * 1) Add the origin point to our active points list.
 * 2) Pick any point from the active points list, and remove it from the list.
 * 3) For all adjacent (non-diagonal) points, if it's ocean (altitude <= 0)
 *    copy our current point's altitude, then reduce it by some amount.
 * 4) If the reduction doesn't put us <= 0, add that point to the active points
 *    list, and set its continent_id to this continent's id (if we've attached
 *    to an existing continent, note that in the joined_continents vector).
 */

	while (!active.empty())
	{
		std::vector<Point> new_points;

		while (!active.empty())
		{
			int index = rng(0, active.size() - 1);
			Point p = active[index];

			for (int i = 0; i < 4; i++)
			{
				int x, y;
				switch (i)
				{
				case 0:
					x = p.x - 1;
					y = p.y;
					break;
				case 1:
					x = p.x + 1;
					y = p.y;
					break;
				case 2:
					x = p.x;
					y = p.y - 1;
					break;
				case 3:
					x = p.x;
					y = p.y + 1;
					break;
				}
				if (x > 0 && x < size && y > 0 && y < size && altitude[x][y] <= 0)
				{
					altitude[x][y] = altitude[p.x][p.y];

// Find the distance to the edge of the map.
					int dist_to_edge = (x < y ? x : y);
					if (size - 1 - x < dist_to_edge)
					{
						dist_to_edge = size - 1 - x;
					}
					if (size - 1 - y < dist_to_edge)
					{
						dist_to_edge = size - 1 - y;
					}

// Slope off quickly if we're close to the edge of the map.
					if (dist_to_edge < 20)
					{
						altitude[x][y] -= rng(0, 2 * (20 - dist_to_edge));

// Occasionally slope off very quickly.
					}
					else if (one_in(30))
					{
						altitude[x][y] -= rng(0, 100);

// Almost always slope off steadily; 1 time in 10 we don't slope off at all!
					}
					else if (!one_in(10))
					{
						altitude[x][y] -= rng(0, step);
					}

					if (altitude[x][y] > 0)
					{
						new_points.push_back(Point(x, y));
						if (continent_id[x][y] != -1)
						{
// Joined continents!
							int other_id = continent_id[x][y];
							if (joined_continents.count(id) == 0)
							{
								std::vector<int> tmp;
								tmp.push_back(other_id);
								joined_continents[id] = tmp;
							}
							else
							{
// Check if we've already noted this
								bool found = false;
								for (int i = 0; !found && i < joined_continents[id].size(); i++)
								{
									if (joined_continents[id][i] == other_id)
									{
										found = true;
									}
								}
								if (!found)
								{
									joined_continents[id].push_back(other_id);
									if (joined_continents.count(other_id) == 0)
									{
										std::vector<int> tmp;
										tmp.push_back(id);
										joined_continents[other_id] = tmp;
									}
									else
									{
										joined_continents[other_id].push_back(id);
									}
								}
							}

						}
						else
						{ // if (continent_id[x][y] != -1)
							continent_id[x][y] = id;
						}

					}
					else
					{ // if (altitude[x][y] > 0)
						altitude[x][y] = 0; // "False" ocean
					}
				} // if (inbounds & altitude <= 0)
			} // for (int i = 0; i < 4; i++)

			active.erase(active.begin() + index);
		} // while (!active.empty())
		active = new_points;
	} // while (!active.empty())

}

void World::add_river(Point origin)
{
	if (OOB(origin))
	{
		return;
	}

	bool done = false;
	while (!done)
	{
		river[origin.x][origin.y] = true;
// Check adjacent tiles for possible next tiles
		int lowest = 500;
		std::vector<Point> candidates;
		for (int x = origin.x - 1; x <= origin.x + 1; x++)
		{
			for (int y = origin.y - 1; y <= origin.y + 1; y++)
			{
				if (x == origin.x && y == origin.y)
				{
					y++;  // Skip the center tile
				}
				if (!OOB(x, y))
				{
					int alt = altitude[x][y];
// Check for adjacent rivers
					int adj_rivers = 0;
					for (int rx = x - 1; rx <= x + 1; rx++)
					{
						for (int ry = y - 1; ry <= y + 1; ry++)
						{
							if (!OOB(rx, ry) && river[rx][ry])
							{
								adj_rivers++;
							}
						}
					}
					alt += 40 * adj_rivers;
					if (alt < lowest && !river[x][y])
					{
						candidates.clear();
						//lowest = altitude[x][y]; // Replace with lowest = alt; ?
						lowest = alt;
						candidates.push_back(Point(x, y));
					}
					else if (alt == lowest && !river[x][y])
					{
						candidates.push_back(Point(x, y));
					}
				}
				else
				{
					done = true;  // Went off the map
				}
			}
		}

// Check if done
		if (candidates.empty())
		{
			done = true;
		}
		for (int i = 0; !done && i < candidates.size(); i++)
		{
			Point p = candidates[i];
			if (altitude[p.x][p.y] <= -1 || river[p.x][p.y])
			{
				done = true;
			}
		}

		if (!done)
		{
			int index = rng(0, candidates.size() - 1);
			origin = candidates[index];
			candidates.erase(candidates.begin() + index);
		}
/*
    if (!candidates.empty()) {
      add_river( candidates[ rng(0, candidates.size() - 1) ] );
    }
*/
	}

}

void World::add_crop(Point origin, Crop crop, int radius)
{
	//crops.push_back( Crop_area(crop, origin, radius) );
	add_resource(origin, crop, MINERAL_NULL, ANIMAL_NULL, radius);
}

void World::add_mineral(Point origin, Mineral mineral, int radius)
{
	//minerals.push_back( Mineral_area(mineral, origin, radius) );
	add_resource(origin, CROP_NULL, mineral, ANIMAL_NULL, radius);
}

void World::add_animal(Point origin, Animal animal, int radius)
{
	add_resource(origin, CROP_NULL, MINERAL_NULL, animal, radius);
}

void World::add_resource(Point origin, Crop crop, Mineral mineral,
		Animal animal, int radius)
{
// Check to make sure that exactly one of crop/mineral/animal is set.
	if (crop == CROP_NULL && mineral == MINERAL_NULL && animal == ANIMAL_NULL)
	{
		Levin::Log::Informational("World::add_resource() called with all resources NULL!");
		return;
	}
	else if (crop != CROP_NULL && mineral != MINERAL_NULL &&
			 animal != ANIMAL_NULL)
	{
		Levin::Log::Informational("World::add_resource() called with all three resources set!");
		return;
	}
	else if (crop != CROP_NULL && mineral != MINERAL_NULL)
	{
		Levin::Log::Informational("World::add_resource() called with crop AND mineral!");
		return;
	}
	else if (crop != CROP_NULL && animal != ANIMAL_NULL)
	{
		Levin::Log::Informational("World::add_resource() called with crop AND animal!");
		return;
	}
	else if (mineral != MINERAL_NULL && animal != ANIMAL_NULL)
	{
		Levin::Log::Informational("World::add_resource() called with mineral AND animal!");
		return;
	}

	if (OOB(origin))
	{
		Levin::Log::Informational("World::add_resource() called with origin %s.",
				origin.str().c_str());
		return;
	}

	if (radius <= 0)
	{
		Levin::Log::Informational("World::add_resource() called with radius {}.", radius);
		return;
	}

// Basically, we're doing the same thing as add_continent(); set the "level" at
// our origin to a high amount, slope downwards in all directions.
	std::vector<Point> active;
	std::vector<Point> placement;
	active.push_back(origin);
	placement.push_back(origin);

	int level[size][size];
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			level[x][y] = 0;
		}
	}

	level[origin.x][origin.y] = 100;
	int step = 100 / radius;

	while (!active.empty())
	{
		std::vector<Point> new_points;
		while (!active.empty())
		{
			int index = rng(0, active.size() - 1);
			Point p = active[index];

			for (int i = 0; i < 4; i++)
			{
				int x, y;
				switch (i)
				{
				case 0:
					x = p.x - 1;
					y = p.y;
					break;
				case 1:
					x = p.x + 1;
					y = p.y;
					break;
				case 2:
					x = p.x;
					y = p.y - 1;
					break;
				case 3:
					x = p.x;
					y = p.y + 1;
					break;
				}
				if (!OOB(x, y) && level[x][y] <= 0)
				{
					level[x][y] = level[p.x][p.y];
					if (one_in(30))
					{
						level[x][y] -= rng(0, 100);
					}
					else if (!one_in(10))
					{
						level[x][y] -= rng(step, step * 2);
					}
					if (level[x][y] > 0)
					{
						new_points.push_back(Point(x, y));
						placement.push_back(Point(x, y));
					}
					else
					{
						level[x][y] = 0;
					}
				}
			}
			active.erase(active.begin() + index);
		}
		active = new_points;
	}

//debugmsg("Placed %d tiles.", placement.size());
// Now place our resource on all points in placement
	for (int i = 0; i < placement.size(); i++)
	{
		int x = placement[i].x, y = placement[i].y;
		if (tiles[x][y] != MAP_NULL && tiles[x][y] != MAP_OCEAN)
		{
			if (crop != CROP_NULL)
			{
// With crops, we need to check to make sure the environment is a match
				if (tile_okay_for_crop(x, y, crop))
				{
					crops[x][y] |= int(pow(2, (int)crop));
				}
			}
			else if (mineral != MINERAL_NULL)
			{
				minerals[x][y] |= int(pow(2, (int)mineral));
			}
			else if (animal != ANIMAL_NULL)
			{
// With animals, we need to check to make sure the environment is a match
				if (tile_okay_for_animal(x, y, animal))
				{
					animals[x][y] |= int(pow(2, (int)animal));
				}
			}
		}
	}
}

void World::update_road_map()
{
	road_map.set_size(size, size);
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			if (has_road(x, y))
			{
				road_map.set_cost(x, y, 0);
			}
			else
			{
				road_map.set_cost(x, y, road_cost(x, y));
			}
		}
	}
}

// traveler defaults to RACE_NULL
void World::update_travel_map(Race traveler)
{
	if (traveler == RACE_NULL)
	{  // Do all the maps!
// Start at 1 to skip RACE_NULL.
		for (int i = 1; i < RACE_MAX; i++)
		{
			Race cur_race = Race(i);
			update_travel_map(cur_race);
		}
		return;
	}

	if (travel_map.count(traveler))
	{
		travel_map[traveler].set_size(size, size);
	}
	else
	{
		travel_map[traveler] = Generic_map(size, size);
	}

	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			if (has_road(x, y))
			{
				travel_map[traveler].set_cost(x, y, 12);
			}
			else
			{
				travel_map[traveler].set_cost(x, y, travel_cost(x, y, traveler));
			}
		}
	}
}

bool World::tile_okay_for_crop(Point p, Crop crop)
{
	return tile_okay_for_crop(p.x, p.y, crop);
}

bool World::tile_okay_for_crop(int x, int y, Crop crop)
{
	Crop_datum* crop_dat = Crop_data[crop];
	return (temperature[x][y] >= crop_dat->min_temp &&
			temperature[x][y] <= crop_dat->max_temp &&
			altitude[x][y] >= crop_dat->min_altitude &&
			altitude[x][y] <= crop_dat->max_altitude &&
			rainfall[x][y] >= crop_dat->min_rainfall &&
			rainfall[x][y] <= crop_dat->max_rainfall);
}

bool World::tile_okay_for_animal(Point p, Animal animal)
{
	return tile_okay_for_animal(p.x, p.y, animal);
}

bool World::tile_okay_for_animal(int x, int y, Animal animal)
{
	Animal_datum* animal_dat = Animal_data[animal];
	return (temperature[x][y] >= animal_dat->min_temp &&
			temperature[x][y] <= animal_dat->max_temp &&
			altitude[x][y] >= animal_dat->min_altitude &&
			altitude[x][y] <= animal_dat->max_altitude &&
			rainfall[x][y] >= animal_dat->min_rainfall &&
			rainfall[x][y] <= animal_dat->max_rainfall);
}

// start defaults to (-1, -1); seen defaults to NULL
Point World::draw(Point start, Map_seen* seen)
{
	cuss::interface i_legend;
	if (!i_legend.load_from_file("cuss/world_legend.cuss"))
	{
		return Point();
	}

	int screen_x, screen_y;
	get_screen_dims(screen_x, screen_y);
	int xdim, ydim;
	xdim = screen_x - 26;
	ydim = screen_y;
	Window w_map(0, 0, xdim, ydim);
	Window w_legend(xdim, 0, 26, ydim);

	if (OOB(start))
	{
		start = continents[0];
	}
	int cur_cont = 0; // Which continent are we on?  So we can use >< to move
	Point pos = start;
	pos.x -= (xdim / 2);  // Pos is in the upper-left corner of our screen
	pos.y -= (ydim / 2);  // So we need to move it from the center to there

	bool hilite_crops = false;
	Crop crop_hilited = CROP_NULL;
	bool hilite_minerals = false;
	Mineral mineral_hilited = MINERAL_NULL;
	bool hilite_animals = false;
	Animal animal_hilited = ANIMAL_NULL;

	while (true)
	{

		Point center(pos.x + xdim / 2, pos.y + ydim / 2);

		for (int x = pos.x; x < pos.x + xdim; x++)
		{
			for (int y = pos.y; y < pos.y + ydim; y++)
			{

				bool tile_seen = (!seen || seen->is_seen(x, y));

				if (!OOB(x, y))
				{
					Map_type type = tiles[x][y];
					Map_type_datum* data = Map_type_data[type];
					if (!data)
					{
						debugmsg("No data for tile[%d][%d] (type %d)!", x, y, type);
					}

					Glyph gl;
					City* city_here = city[x][y];

					if (!tile_seen)
					{
						gl = Glyph('-', c_dkgray, c_black);
					}
					else if (city_here)
					{
						gl = city_here->get_glyph();
					}
					else if (has_road(x, y))
					{
						gl = get_road_glyph(x, y);
					}
					else
					{
						gl = data->symbol;
					}

					int kingdom_id = get_kingdom_id(x, y);
					if (!city_here && kingdom_id >= 0 && tile_seen &&
							kingdom_id < GAME->kingdoms.size() &&
							!hilite_crops && !hilite_minerals && !hilite_animals)
					{
						Kingdom* kingdom = GAME->kingdoms[kingdom_id];
// Skip adding the kingdom background if it would interfere with our cursor.
						gl = gl.hilite(kingdom->color);
					}

					bool do_crop_hilite = (hilite_crops &&
							has_crop(crop_hilited, x, y));
					bool do_mineral_hilite = (hilite_minerals &&
							has_mineral(mineral_hilited, x, y));
					bool do_animal_hilite = (hilite_animals &&
							has_animal(animal_hilited, x, y));

// See if we need to change the background color for any reason.
					if (!city_here && tile_seen)
					{ // No highlighting if there's a city
						if (do_crop_hilite && do_mineral_hilite && do_animal_hilite)
						{
							gl = gl.hilite(c_ltgray);
						}
						else if (do_crop_hilite && do_mineral_hilite)
						{
							gl = gl.hilite(c_brown);
						}
						else if (do_crop_hilite && do_animal_hilite)
						{
							gl = gl.hilite(c_cyan);
						}
						else if (do_mineral_hilite && do_animal_hilite)
						{
							gl = gl.hilite(c_magenta);
						}
						else if (do_crop_hilite)
						{
							gl = gl.hilite(c_green);
						}
						else if (do_mineral_hilite)
						{
							gl = gl.hilite(c_red);
						}
						else if (do_animal_hilite && (x != center.x || y != center.y))
						{
							gl = gl.hilite(c_blue);
						}
					}
// Highlighting the center tile takes precedence over everything else
					if (x == center.x && y == center.y)
					{
						gl = gl.hilite(c_blue);
					}

					w_map.putglyph(x - pos.x, y - pos.y, gl);

				}
				else
				{  // Out of bounds Glyph
					w_map.putglyph(x - pos.x, y - pos.y, Glyph('-', c_dkgray, c_black));
				}

			} // for (int y = pos.y; y < pos.y + ydim; y++)
		} // for (int x = pos.x; x < pos.x + xdim; x++)

// Now draw the legend
		Map_type type = get_map_type(center);
		Map_type_datum* data = Map_type_data[type];
		i_legend.set_data("text_position", center.str());
		i_legend.set_data("text_position", c_white);
		bool tile_seen = (!seen || seen->is_seen(center));
		if (tile_seen)
		{
			i_legend.set_data("text_map_type", data->name);
			i_legend.set_data("text_map_type", data->symbol.fg);
		}
		else
		{
			i_legend.set_data("text_map_type", "<c=dkgray>Unknown<c=/>");
		}
/* We want two crops/minerals per line, so I split the text fields into two.
 * Each one has its own stringstream; so we put the first crop/mineral into the
 * left stringstream/field, the second into the right, etc.
 */
		if (tile_seen)
		{
			std::stringstream crops_left_ss, minerals_left_ss,
					crops_right_ss, minerals_right_ss;

			std::vector<Crop> crops_here = crops_at(center);
			std::vector<Mineral> minerals_here = minerals_at(center);

			for (int i = 0; i < crops_here.size(); i++)
			{
				std::stringstream* crop_ss;
				if (i % 2 == 0)
				{
					crop_ss = &(crops_left_ss);
				}
				else
				{
					crop_ss = &(crops_right_ss);
				}
				Crop_datum* crop_dat = Crop_data[crops_here[i]];
				nc_color crop_color = crop_type_color(crop_dat->type);
				(*crop_ss) << "<c=" << color_tag(crop_color) << ">" << crop_dat->name <<
																					  "<c=/>" << std::endl;
			}
			for (int i = 0; i < minerals_here.size(); i++)
			{
				std::stringstream* mineral_ss;
				if (i % 2 == 0)
				{
					mineral_ss = &(minerals_left_ss);
				}
				else
				{
					mineral_ss = &(minerals_right_ss);
				}
				Mineral_datum* mineral_dat = Mineral_data[minerals_here[i]];
				nc_color mineral_color = mineral_dat->color;
				(*mineral_ss) << "<c=" << color_tag(mineral_color) << ">" <<
																		  mineral_dat->name << "<c=/>" << std::endl;
			}
			i_legend.set_data("text_crops_here_left", crops_left_ss.str());
			i_legend.set_data("text_crops_here_right", crops_right_ss.str());
			i_legend.set_data("text_minerals_here_left", minerals_left_ss.str());
			i_legend.set_data("text_minerals_here_right", minerals_right_ss.str());

// Kingdom info
			int kingdom_id = get_kingdom_id(center);
			i_legend.set_data("num_kingdom_id", kingdom_id);
			if (kingdom_id >= 0 && kingdom_id < GAME->kingdoms.size())
			{
				Kingdom* kingdom = GAME->kingdoms[kingdom_id];
				Race_datum* race_dat = Race_data[kingdom->race];
				std::stringstream ss_race;
				ss_race << "<c=" << color_tag(race_dat->color) << ">" <<
																	  capitalize(race_dat->plural_name) << "<c=/>";
				i_legend.set_data("text_kingdom_race", ss_race.str());
			}
			else
			{
				i_legend.set_data("text_kingdom_race", "<c=dkgray>None<c=/>");
			}

// City info
			City* city_here = get_city(center);
			if (city_here)
			{
				i_legend.set_data("text_city_name", city_here->get_name());
				i_legend.set_data("text_city_name", c_yellow);
			}
			else
			{
				i_legend.clear_data("text_city_name");
			}
		}
		else
		{  // Tile is not seen
			i_legend.clear_data("text_crops_here_left");
			i_legend.clear_data("text_crops_here_right");
			i_legend.clear_data("text_minerals_here_left");
			i_legend.clear_data("text_minerals_here_right");
			i_legend.clear_data("num_kingdom_id");
			i_legend.clear_data("text_kingdom_race");
			i_legend.clear_data("text_city-name");
		}

		i_legend.draw(&w_legend);

		w_legend.refresh();
		w_map.refresh();

		long ch = getch();
// true in input_direction() means we accept capital letters
		Point move_dir = input_direction(ch, true);
// ...but we want capital letters to move TEN tiles in the given direction.
// If ch was not a movement key, move_dir.x will equal -2.
		if (move_dir.x != -2 && ch >= 'A' && ch <= 'Z')
		{
			move_dir.x *= 10;
			move_dir.y *= 10;
		}
		if (move_dir.x != -2)
		{
			pos += move_dir;
		}
		else
		{
			switch (ch)
			{
			case '0':
				pos = start;
				pos.x -= (xdim / 2); // Pos is in the upper-left corner of our screen
				pos.y -= (ydim / 2); // So we need to move it from the center to there
				break;

			case '>':
				if (cur_cont >= continents.size() - 1)
				{
					cur_cont = 0;
				}
				else
				{
					cur_cont++;
				}
				pos.x = continents[cur_cont].x - (xdim / 2);
				pos.y = continents[cur_cont].y - (ydim / 2);
				break;

			case '<':
				if (cur_cont <= 0)
				{
					cur_cont = continents.size() - 1;
				}
				else
				{
					cur_cont--;
				}
				pos.x = continents[cur_cont].x - (xdim / 2);
				pos.y = continents[cur_cont].y - (ydim / 2);
				break;

			case 'c':
			case 'C':
			{
				std::string crop_name = string_input_popup("hilite crop:");
				Crop hilited = CROP_NULL;
				bool do_hilite = true;
				if (!crop_name.empty())
				{
					hilited = search_for_crop(crop_name);
					if (hilited == CROP_NULL)
					{
						popup("%s not found.");
						do_hilite = false;
					}
				}
				if (do_hilite)
				{
					hilite_crops = true;
					//hilite_minerals = false;
					crop_hilited = hilited;
				}
				else
				{
					hilite_crops = false;
				}
			}
				break;

			case 'm':
			case 'M':
			{
				std::string mineral_name = string_input_popup("hilite mineral:");
				Mineral hilited = MINERAL_NULL;
				bool do_hilite = true;
				if (!mineral_name.empty())
				{
					hilited = search_for_mineral(mineral_name);
					if (hilited == MINERAL_NULL)
					{
						popup("%s not found.");
						do_hilite = false;
					}
				}
				if (do_hilite)
				{
					hilite_minerals = true;
					//hilite_crops = false;
					mineral_hilited = hilited;
				}
				else
				{
					hilite_minerals = false;
				}
			}
				break;

			case 'a':
			case 'A':
			{
				std::string animal_name = string_input_popup("hilite animal:");
				Animal hilited = ANIMAL_NULL;
				bool do_hilite = true;
				if (!animal_name.empty())
				{
					hilited = search_for_animal(animal_name);
					if (hilited == ANIMAL_NULL)
					{
						popup("%s not found.");
						do_hilite = false;
					}
				}
				if (do_hilite)
				{
					hilite_animals = true;
					//hilite_crops = false;
					animal_hilited = hilited;
				}
				else
				{
					hilite_animals = false;
				}
			}
				break;

			case 't':
			case 'T':
				hilite_crops = false;
				hilite_minerals = false;
				hilite_animals = false;
				break;

			case '?':
			{
				City* city_checked = get_city(center);
				if (city_checked)
				{
					AI_city* ai_city = static_cast<AI_city*>(city_checked);
					popup_fullscreen(ai_city->list_production().c_str());
				}
				else
				{
					std::stringstream debug_info;
					debug_info << "Altitude: " << altitude[center.x][center.y] <<
																			   std::endl <<
																			   "Rainfall: " << rainfall[center.x][center.y] <<
																			   std::endl <<
																			   "Temperature: " << temperature[center.x][center.y];
					popup_fullscreen(debug_info.str().c_str());
				}

/*
          debugmsg("%s\nRiver Start: %s\nRiver End:   %s",
                   (is_river(center) ? "River" : "Not River"),
                   Direction_name(river_start_for(center)).c_str(),
                   Direction_name(river_end_for  (center)).c_str());
*/
			}
				break;

			case KEY_ESC:
			case 'q':
			case 'Q':
			case '\n':
				if (ch == '\n')
				{
					pos.x += (xdim / 2);
					pos.y += (ydim / 2);
					return pos;
				}
				return Point(-1, -1);
			} // switch (ch)
		} // if (move_dir.x == -2)
	} // while (true)
}

std::string World::get_name()
{
	return name;
}

int World::get_size()
{
	return size;
}

bool World::OOB(Point p)
{
	return OOB(p.x, p.y);
}

bool World::OOB(int x, int y)
{
	return (x < 0 || x >= size ||
			y < 0 || y >= size);
}

Map_type World::get_map_type(Point p)
{
	return get_map_type(p.x, p.y);
}

Map_type World::get_map_type(int x, int y)
{
	if (OOB(x, y))
	{
		return MAP_NULL;
	}
	return tiles[x][y];
}

void World::set_kingdom_id(Point p, int id)
{
	set_kingdom_id(p.x, p.y, id);
}

void World::set_kingdom_id(int x, int y, int id)
{
	if (OOB(x, y))
	{
		return;
	}
	kingdom_id[x][y] = id;
}

int World::get_kingdom_id(Point p)
{
	return get_kingdom_id(p.x, p.y);
}

int World::get_kingdom_id(int x, int y)
{
	if (OOB(x, y))
	{
		return -1;
	}
	return kingdom_id[x][y];
}

void World::set_city(Point p, City* new_city)
{
	set_city(p.x, p.y, new_city);
}

void World::set_city(int x, int y, City* new_city)
{
	if (!new_city || OOB(x, y))
	{
		return;
	}
// TODO: Is this necessary?  It shouldn't take THAT long but it's basically just
//       a check for mistakes in the code.
	for (int i = 0; i < city_list.size(); i++)
	{
		if (city_list[i] == new_city)
		{
			debugmsg("Adding a city to %d:%d but that city already exists at %d:%d!",
					x, y, new_city->location.x, new_city->location.y);
			return;
		}
	}
	city[x][y] = new_city;
	city_list.push_back(new_city);
}

City* World::get_city(Point p)
{
	return get_city(p.x, p.y);
}

City* World::get_city(int x, int y)
{
	if (OOB(x, y))
	{
		return NULL;
	}
	return city[x][y];
}

City* World::lookup_city_uid(int uid)
{
	for (int i = 0; i < city_list.size(); i++)
	{
		if (city_list[i] && city_list[i]->uid == uid)
		{
			return city_list[i];
		}
	}
	return NULL;
}

bool World::has_road(int x, int y)
{
	if (OOB(x, y))
	{
		return false;
	}
	return (road[x][y] || get_city(x, y));
}

int World::road_cost(Point p)
{
	return road_cost(p.x, p.y);
}

int World::road_cost(int x, int y)
{
	if (OOB(x, y))
	{
		return -1; // < 0 means "roads are forbidden"
	}
	if (has_road(x, y))
	{
		return 0; // TODO: Should this be 0?
	}
	return Map_type_data[get_map_type(x, y)]->road_cost;
}

// traveler defaults to RACE_NULL
int World::travel_cost(Point p, Race traveler)
{
	return travel_cost(p.x, p.y, traveler);
}

// traveler defaults to RACE_NULL
int World::travel_cost(int x, int y, Race traveler)
{
	if (OOB(x, y))
	{
		return -1;
	}
	Map_type terrain = get_map_type(x, y);
	if (traveler != RACE_NULL)
	{
		Race_datum* race_dat = Race_data[traveler];
		if (race_dat->map_type_travel_cost.count(terrain))
		{
			return race_dat->map_type_travel_cost[terrain];
		}
	}
	return Map_type_data[terrain]->travel_cost;
}

int World::route_cost(Race traveler, Point start, Point end)
{
	if (traveler == RACE_NULL)
	{  // Sanity check
		Levin::Log::Informational("World::route_cost(RACE_NULL, ...) called!");
		return -1;
	}
	if (start == end)
	{
		return -1;
	}
// We should ALWAYS have a map for each race, but check just in case.
	if (travel_map.count(traveler) == 0)
	{
		debugmsg("For some reason World doesn't have a travel_map for %s!",
				Race_data[traveler]->name.c_str());
		return -1;
	}

// See pathfind.h / pathfind.cpp
	Pathfinder pf(travel_map[traveler]);
	Path route = pf.get_path(PATH_A_STAR, start, end);

	if (route.empty())
	{
		return -1;
	}
	return route.get_cost();
}

Glyph World::get_road_glyph(int x, int y)
{
	if (!has_road(x, y))
	{  // has_road() checks for OOB
		return Glyph();
	}

	Glyph ret(LINE_XXXX, c_white, c_black);

	if (has_road(x, y - 1))
	{
		if (has_road(x + 1, y))
		{
			if (has_road(x, y + 1))
			{
				if (has_road(x - 1, y))
				{
					ret.symbol = LINE_XXXX; // Redundant given the declaration but oh well
				}
				else
				{
					ret.symbol = LINE_XXXO;
				}
			}
			else
			{  // No southbound
				if (has_road(x - 1, y))
				{
					ret.symbol = LINE_XXOX;
				}
				else
				{
					ret.symbol = LINE_XXOO;
				}
			}
		}
		else
		{  // No eastbound
			if (has_road(x, y + 1))
			{
				if (has_road(x - 1, y))
				{
					ret.symbol = LINE_XOXX;
				}
				else
				{
					ret.symbol = LINE_XOXO;
				}
			}
			else
			{  // No southbound
				if (has_road(x - 1, y))
				{
					ret.symbol = LINE_XOOX;
				}
				else
				{
					ret.symbol = LINE_XOXO;
				}
			}
		}
	}
	else
	{  // No northbound
		if (has_road(x + 1, y))
		{
			if (has_road(x, y + 1))
			{
				if (has_road(x - 1, y))
				{
					ret.symbol = LINE_OXXX; // Redundant given the declaration but oh well
				}
				else
				{
					ret.symbol = LINE_OXXO;
				}
			}
			else
			{  // No southbound
// Only East and West & East are both horizontal lines
				ret.symbol = LINE_OXOX;
			}
		}
		else
		{  // No eastbound
			if (has_road(x, y + 1))
			{
				if (has_road(x - 1, y))
				{
					ret.symbol = LINE_OOXX;
				}
				else
				{
					ret.symbol = LINE_XOXO;
				}
			}
			else
			{  // No southbound
				if (has_road(x - 1, y))
				{
					ret.symbol = LINE_OXOX;
				}
				else
				{  // No neighbors at all!
					ret.symbol = LINE_XXXX;
				}
			}
		}
	}

	return ret;
}

bool World::is_river(Point p)
{
	return is_river(p.x, p.y);
}

bool World::is_river(int x, int y)
{
	Map_type type = get_map_type(x, y);
	return Map_type_data[type]->is_river;
}

bool World::is_water(Point p)
{
	return is_water(p.x, p.y);
}

bool World::is_water(int x, int y)
{
	Map_type type = get_map_type(x, y);
	return Map_type_data[type]->is_water;
}

Direction World::coast_from(Point p)
{
	return coast_from(p.x, p.y);
}

Direction World::coast_from(int x, int y)
{
	if (OOB(x, y))
	{
		return DIR_NULL;
	}
	std::vector<Direction> candidates;
	if (get_map_type(x - 1, y) == MAP_OCEAN)
	{
		candidates.push_back(DIR_WEST);
	}
	if (get_map_type(x + 1, y) == MAP_OCEAN)
	{
		candidates.push_back(DIR_EAST);
	}
	if (get_map_type(x, y - 1) == MAP_OCEAN)
	{
		candidates.push_back(DIR_NORTH);
	}
	if (get_map_type(x, y + 1) == MAP_OCEAN)
	{
		candidates.push_back(DIR_SOUTH);
	}

	if (candidates.empty())
	{
		return DIR_NULL;
	}
	return candidates[rng(0, candidates.size() - 1)];
}

Direction_full World::river_start_for(Point p)
{
	return river_start_for(p.x, p.y);
}

Direction_full World::river_start_for(int x, int y)
{
// Bounds check
	if (OOB(x, y))
	{
		return DIRFULL_NULL;
	}
// Check in this order: northwest, west/north (random), southwest, northeast
	if (is_water(x - 1, y - 1))
	{
		return DIRFULL_NORTHWEST;
	}
	std::vector<Direction_full> options;
	if (is_water(x - 1, y))
	{
		options.push_back(DIRFULL_WEST);
	}
	if (is_water(x, y - 1))
	{
		options.push_back(DIRFULL_NORTH);
	}
	if (!options.empty())
	{
		return options[rng(0, options.size() - 1)];
	}
	if (is_water(x + 1, y - 1))
	{
		return DIRFULL_NORTHEAST;
	}
	if (is_water(x - 1, y + 1))
	{
		return DIRFULL_SOUTHWEST;
	}
	return DIRFULL_NULL;
}

Direction_full World::river_end_for(Point p)
{
	return river_end_for(p.x, p.y);
}

Direction_full World::river_end_for(int x, int y)
{
// Bounds check
	if (OOB(x, y))
	{
		return DIRFULL_NULL;
	}
// Check in this order: southeast, south/east (random), southwest, northeast
// Check river_start_for() to ensure we don't return the same value!
	if (is_water(x + 1, y + 1))
	{
		return DIRFULL_SOUTHEAST;
	}
	std::vector<Direction_full> options;
	if (is_water(x, y + 1))
	{
		options.push_back(DIRFULL_SOUTH);
	}
	if (is_water(x + 1, y))
	{
		options.push_back(DIRFULL_EAST);
	}
	if (!options.empty())
	{
		return options[rng(0, options.size() - 1)];
	}
	Direction_full river_start = river_start_for(x, y);
	if (is_water(x - 1, y + 1) && river_start != DIRFULL_SOUTHWEST)
	{
		return DIRFULL_SOUTHWEST;
	}
	if (is_water(x + 1, y - 1) && river_start != DIRFULL_NORTHEAST)
	{
		return DIRFULL_NORTHEAST;
	}
	return DIRFULL_NULL;
}

int World::get_trade_distance(Race trader, Point start, Point end)
{
/* TODO:  Would anything increase the trade distance?
          Enemy races we have to avoid
          Areas with bandits/monsters
 */
	return route_cost(trader, start, end);
}

bool World::build_road(Point start, Point end)
{
// See pathfind.h / pathfind.cpp
	Pathfinder pf(road_map);
	Path route = pf.get_path(PATH_A_STAR, start, end);
	if (route.empty())
	{ // Impossible to build the road!
		return false;
	}

	for (int i = 0; i < route.size(); i++)
	{
		if (!OOB(route[i]))
		{
			road[route[i].x][route[i].y] = true;
// Update our road map.
			road_map.set_cost(route[i].x, route[i].y, 0);
		}
	}
	return true;
}

std::vector<Crop> World::crops_at(Point p)
{
	return crops_at(p.x, p.y);
}

std::vector<Mineral> World::minerals_at(Point p)
{
	return minerals_at(p.x, p.y);
}

std::vector<Animal> World::animals_at(Point p)
{
	return animals_at(p.x, p.y);
}

std::vector<Crop> World::crops_at(int x, int y)
{
	std::vector<Crop> ret;
	if (OOB(x, y))
	{
		return ret;
	}
	for (int i = 1; i < CROP_MAX; i++)
	{
		if (has_crop(Crop(i), x, y))
		{
			ret.push_back(Crop(i));
		}
	}
	return ret;
}

std::vector<Mineral> World::minerals_at(int x, int y)
{
	std::vector<Mineral> ret;
	if (OOB(x, y))
	{
		return ret;
	}
	for (int i = 1; i < MINERAL_MAX; i++)
	{
		if (has_mineral(Mineral(i), x, y))
		{
			ret.push_back(Mineral(i));
		}
	}
	return ret;
}

std::vector<Animal> World::animals_at(int x, int y)
{
	std::vector<Animal> ret;
	if (OOB(x, y))
	{
		return ret;
	}
	for (int i = 1; i < ANIMAL_MAX; i++)
	{
		if (has_animal(Animal(i), x, y))
		{
			ret.push_back(Animal(i));
		}
	}
	return ret;
}

bool World::has_crop(Crop crop, int x, int y)
{
	if (OOB(x, y))
	{
		return false;
	}
	if (crop == CROP_NULL && crops[x][y] > 1)
	{
		return true;
	}
	return (crops[x][y] & int(pow(2, (int)crop)));
}

bool World::has_mineral(Mineral mineral, int x, int y)
{
	if (OOB(x, y))
	{
		return false;
	}
	if (mineral == MINERAL_NULL && minerals[x][y] > 1)
	{
		return true;
	}
	return (minerals[x][y] & int(pow(2, (int)mineral)));
}

bool World::has_animal(Animal animal, int x, int y)
{
	if (OOB(x, y))
	{
		return false;
	}
	if (animal == ANIMAL_NULL && animals[x][y] > 1)
	{
		return true;
	}
	return (animals[x][y] & int(pow(2, (int)animal)));
}

int world_size_kingdoms(World_size size)
{
	switch (size)
	{
	case WORLD_SIZE_SMALL:
		return 2;
	case WORLD_SIZE_MEDIUM:
		return 5;
	case WORLD_SIZE_LARGE:
		return 10;
	case WORLD_SIZE_HUGE:
		return 14;
	default:
		return 0;
	}
	return 0;
}

int world_size_size(World_size size)
{
	switch (size)
	{
	case WORLD_SIZE_SMALL:
		return 50;
	case WORLD_SIZE_MEDIUM:
		return 100;
	case WORLD_SIZE_LARGE:
		return 200;
	case WORLD_SIZE_HUGE:
		return 400;
	default:
		return 100;
	}
	return 100;
}


std::string world_size_name(World_size size)
{
	switch (size)
	{
	case WORLD_SIZE_SMALL:
		return "Small";
	case WORLD_SIZE_MEDIUM:
		return "Medium";
	case WORLD_SIZE_LARGE:
		return "Large";
	case WORLD_SIZE_HUGE:
		return "Huge";
	case WORLD_SIZE_MAX:
		return "BUG - WORLD_SIZE_MAX";
	default:
		return "BUG - Unnamed World_size";
	}
	return "BUG - Escaped world_size_name() switch";
}

std::string world_temperature_name(World_temperature temp)
{
	switch (temp)
	{
	case WORLD_TEMP_ICE_AGE:
		return "Ice Age";
	case WORLD_TEMP_COLD:
		return "Cold";
	case WORLD_TEMP_TEMPERATE:
		return "Temperate";
	case WORLD_TEMP_HOT:
		return "Hot";
	case WORLD_TEMP_FURNACE:
		return "Furnace";
	case WORLD_TEMP_MAX:
		return "BUG - WORLD_TEMP_MAX";
	default:
		return "BUG - Unnamed World_temperature";
	}
	return "BUG - Escaped world_temperature_name() switch";
}

nc_color world_temperature_color(World_temperature temp)
{
	switch (temp)
	{
	case WORLD_TEMP_ICE_AGE:
		return c_white;
	case WORLD_TEMP_COLD:
		return c_ltcyan;
	case WORLD_TEMP_TEMPERATE:
		return c_ltgreen;
	case WORLD_TEMP_HOT:
		return c_yellow;
	case WORLD_TEMP_FURNACE:
		return c_ltred;
	case WORLD_TEMP_MAX:
		return c_magenta;
	default:
		return c_magenta;
	}
	return c_magenta;
}

std::string world_rainfall_name(World_rainfall rain)
{
	switch (rain)
	{
	case WORLD_RAIN_DRY:
		return "Dry";
	case WORLD_RAIN_LIGHT:
		return "Light";
	case WORLD_RAIN_MODERATE:
		return "Moderate";
	case WORLD_RAIN_HEAVY:
		return "Heavy";
	case WORLD_RAIN_UNENDING:
		return "Unending";
	case WORLD_RAIN_MAX:
		return "BUG - WORLD_RAIN_MAX";
	default:
		return "BUG - Unnamed World_rainfall";
	}
	return "BUG -Escaped world_rainfall_name() switch";
}

nc_color world_rainfall_color(World_rainfall rain)
{
	switch (rain)
	{
	case WORLD_RAIN_DRY:
		return c_ltred;
	case WORLD_RAIN_LIGHT:
		return c_yellow;
	case WORLD_RAIN_MODERATE:
		return c_ltcyan;
	case WORLD_RAIN_HEAVY:
		return c_cyan;
	case WORLD_RAIN_UNENDING:
		return c_blue;
	case WORLD_RAIN_MAX:
		return c_magenta;
	default:
		return c_magenta;
	}
	return c_magenta;
}

std::string world_mountain_name(World_mountain mountain)
{
	switch (mountain)
	{
	case WORLD_MOUNTAIN_FLAT:
		return "Flat";
	case WORLD_MOUNTAIN_LOW:
		return "Low";
	case WORLD_MOUNTAIN_VARIED:
		return "Varied";
	case WORLD_MOUNTAIN_HIGH:
		return "High";
	case WORLD_MOUNTAIN_COVERED:
		return "Covered";
	case WORLD_MOUNTAIN_MAX:
		return "BUG - WORLD_MOUNTAIN_MAX";
	default:
		return "Unnamed World_mountain";
	}
	return "BUG - Escaped world_mountain_name() switch";
}

nc_color world_mountain_color(World_mountain mountain)
{
	switch (mountain)
	{
	case WORLD_MOUNTAIN_FLAT:
		return c_ltblue;
	case WORLD_MOUNTAIN_LOW:
		return c_green;
	case WORLD_MOUNTAIN_VARIED:
		return c_yellow;
	case WORLD_MOUNTAIN_HIGH:
		return c_ltgray;
	case WORLD_MOUNTAIN_COVERED:
		return c_ltred;
	case WORLD_MOUNTAIN_MAX:
		return c_magenta;
	default:
		return c_magenta;
	}
	return c_magenta;
}

std::string get_random_world_name()
{
	std::stringstream ss_name;

	bool double_vowel = false;

	switch (rng(1, 5))
	{
	case 1: // Start with a vowel
		switch (rng(1, 5))
		{
		case 1:
			ss_name << "A";
			break;
		case 2:
			ss_name << "E";
			break;
		case 3:
			ss_name << "I";
			break;
		case 4:
			ss_name << "O";
			break;
		case 5:
			ss_name << "U";
			break;
		}
		break;

	case 2:
	case 3:
	case 4:
	case 5: // Start with a consonant
		switch (rng(1, 19))
		{
		case 1:
			ss_name << "B";
			break;
		case 2:
			ss_name << "C";
			break;
		case 3:
			ss_name << "D";
			break;
		case 4:
			ss_name << "F";
			break;
		case 5:
			ss_name << "G";
			break;
		case 6:
			ss_name << "H";
			break;
		case 7:
			ss_name << "J";
			break;
		case 8:
			ss_name << "K";
			break;
		case 9:
			ss_name << "L";
			break;
		case 10:
			ss_name << "M";
			break;
		case 11:
			ss_name << "N";
			break;
		case 12:
			ss_name << "P";
			break;
		case 13:
			ss_name << "Q";
			break;
		case 14:
			ss_name << "R";
			break;
		case 15:
			ss_name << "S";
			break;
		case 16:
			ss_name << "T";
			break;
		case 17:
			ss_name << "V";
			break;
		case 18:
			ss_name << "W";
			break;
		case 19:
			ss_name << "Y";
			break;
		}
		if (ss_name.str()[0] == 'Q')
		{ // Only use u
			double_vowel = true;
			switch (rng(1, 3))
			{
			case 1:
				ss_name << "ua";
				break;
			case 2:
				ss_name << "ui";
				break;
			case 3:
				ss_name << "uo";
				break;
			}
		}
		else
		{  // Any vowel will do
			switch (rng(1, 5))
			{
			case 1:
				ss_name << "a";
				break;
			case 2:
				ss_name << "e";
				break;
			case 3:
				ss_name << "i";
				break;
			case 4:
				ss_name << "o";
				break;
			case 5:
				ss_name << "u";
				break;
			}
		}
		break;
	} // Vowel or consonant?

// Now we have a first syllable that ends in a vowel; add a consonant
	std::string cons;
	if (double_vowel)
	{
		switch (rng(1, 13))
		{
		case 1:
			cons = "b";
			break;
		case 2:
			cons = "c";
			break;
		case 3:
			cons = "d";
			break;
		case 4:
			cons = "f";
			break;
		case 5:
			cons = "l";
			break;
		case 6:
			cons = "m";
			break;
		case 7:
			cons = "n";
			break;
		case 8:
			cons = "p";
			break;
		case 9:
			cons = "r";
			break;
		case 10:
			cons = "s";
			break;
		case 11:
			cons = "t";
			break;
		case 12:
			cons = "v";
			break;
		case 13:
			cons = "z";
			break;
		}
	}
	else
	{  // Not a double-vowel
		switch (rng(1, 54))
		{
		case 1:
			cons = "bb";
			break;
		case 2:
			cons = "bl";
			break;
		case 3:
			cons = "br";
			break;
		case 4:
			cons = "ch";
			break;
		case 5:
			cons = "ck";
			break;
		case 6:
			cons = "cl";
			break;
		case 7:
			cons = "cr";
			break;
		case 8:
			cons = "dd";
			break;
		case 9:
			cons = "dr";
			break;
		case 10:
			cons = "ff";
			break;
		case 11:
			cons = "fl";
			break;
		case 12:
			cons = "fr";
			break;
		case 13:
			cons = "gl";
			break;
		case 14:
			cons = "gr";
			break;
		case 15:
			cons = "kl";
			break;
		case 16:
			cons = "kr";
			break;
		case 17:
			cons = "ll";
			break;
		case 18:
			cons = "mm";
			break;
		case 19:
			cons = "mn";
			break;
		case 20:
			cons = "nd";
			break;
		case 21:
			cons = "ng";
			break;
		case 22:
			cons = "nj";
			break;
		case 23:
			cons = "nk";
			break;
		case 24:
			cons = "nn";
			break;
		case 25:
			cons = "np";
			break;
		case 26:
			cons = "ns";
			break;
		case 27:
			cons = "nt";
			break;
		case 28:
			cons = "pl";
			break;
		case 29:
			cons = "pr";
			break;
		case 30:
			cons = "rd";
			break;
		case 31:
			cons = "rf";
			break;
		case 32:
			cons = "rg";
			break;
		case 33:
			cons = "rk";
			break;
		case 34:
			cons = "rm";
			break;
		case 35:
			cons = "rn";
			break;
		case 36:
			cons = "rp";
			break;
		case 37:
			cons = "rq";
			break;
		case 38:
			cons = "rr";
			break;
		case 39:
			cons = "rs";
			break;
		case 40:
			cons = "rt";
			break;
		case 41:
			cons = "rz";
			break;
		case 42:
			cons = "sb";
			break;
		case 43:
			cons = "sd";
			break;
		case 44:
			cons = "sh";
			break;
		case 45:
			cons = "sk";
			break;
		case 46:
			cons = "sm";
			break;
		case 47:
			cons = "sp";
			break;
		case 48:
			cons = "ss";
			break;
		case 49:
			cons = "st";
			break;
		case 50:
			cons = "th";
			break;
		case 51:
			cons = "tr";
			break;
		case 52:
			cons = "tt";
			break;
		case 53:
			cons = "x";
			break;
		case 54:
			cons = "zz";
			break;
		case 55:
			cons = "q";
			break;
		}
	}
	ss_name << cons;

	bool used_r = false;  // We only want one 'r' in the word
	if (cons.find('r') != std::string::npos)
	{
		used_r = true;
	}

// Now start syllable 2 with a vowel
	if (cons[cons.size() - 1] == 'q')
	{ // Only use u
		switch (rng(1, 3))
		{
		case 1:
			ss_name << "ua";
			break;
		case 2:
			ss_name << "ui";
			break;
		case 3:
			ss_name << "uo";
			break;
		}
	}
	else
	{  // Any vowel will do
		switch (rng(1, 4))
		{
		case 1:
			ss_name << "a";
			break;
		case 2:
			ss_name << "e";
			break;
		case 3:
			ss_name << "i";
			break;
		case 4:
			ss_name << "o";
			break;
		}
	}

// Final random consonant
	int max = (used_r ? 22 : 33);
	switch (rng(1, max))
	{
	case 1:
		ss_name << "b";
		break;
	case 2:
		ss_name << "ch";
		break;
	case 3:
		ss_name << "ck";
		break;
	case 4:
		ss_name << "d";
		break;
	case 5:
		ss_name << "ft";
		break;
	case 6:
		ss_name << "g";
		break;
	case 7:
		ss_name << "k";
		break;
	case 8:
		ss_name << "kl";
		break;
	case 9:
		ss_name << "l";
		break;
	case 10:
		ss_name << "m";
		break;
	case 11:
		ss_name << "n";
		break;
	case 12:
		ss_name << "p";
		break;
	case 13:
		ss_name << "s";
		break;
	case 14:
		ss_name << "sh";
		break;
	case 15:
		ss_name << "sk";
		break;
	case 16:
		ss_name << "sp";
		break;
	case 17:
		ss_name << "ss";
		break;
	case 18:
		ss_name << "st";
		break;
	case 19:
		ss_name << "t";
		break;
	case 20:
		ss_name << "th";
		break;
	case 21:
		ss_name << "tt";
		break;
	case 22:
		ss_name << "v";
		break;
	case 23:
		ss_name << "z";
		break;
	case 24:
		ss_name << "r";
		break;
	case 25:
		ss_name << "rd";
		break;
	case 26:
		ss_name << "rg";
		break;
	case 27:
		ss_name << "rk";
		break;
	case 28:
		ss_name << "rm";
		break;
	case 29:
		ss_name << "rn";
		break;
	case 30:
		ss_name << "rp";
		break;
	case 31:
		ss_name << "rr";
		break;
	case 32:
		ss_name << "rt";
		break;
	case 33:
		ss_name << "tr";
		break;
	}

// And then an ending
// 25 - 30 are "no ending"
	switch (rng(1, 30))
	{
	case 1:
		ss_name << "a";
		break;
	case 2:
		ss_name << "ade";
		break;
	case 3:
		ss_name << "agos";
		break;
	case 4:
		ss_name << "ale";
		break;
	case 5:
		ss_name << "am";
		break;
	case 6:
		ss_name << "ane";
		break;
	case 7:
		ss_name << "ate";
		break;
	case 8:
		ss_name << "ea";
		break;
	case 9:
		ss_name << "el";
		break;
	case 10:
		ss_name << "em";
		break;
	case 11:
		ss_name << "ene";
		break;
	case 12:
		ss_name << "ers";
		break;
	case 13:
		ss_name << "ia";
		break;
	case 14:
		ss_name << "ile";
		break;
	case 15:
		ss_name << "ilia";
		break;
	case 16:
		ss_name << "ire";
		break;
	case 17:
		ss_name << "ist";
		break;
	case 18:
		ss_name << "oa";
		break;
	case 19:
		ss_name << "oga";
		break;
	case 20:
		ss_name << "ogi";
		break;
	case 21:
		ss_name << "os";
		break;
	case 22:
		ss_name << "ua";
		break;
	case 23:
		ss_name << "une";
		break;
	case 24:
		ss_name << "use";
		break;
	}

	return ss_name.str();
}

