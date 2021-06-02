#ifndef _KINGDOM_H_
#define _KINGDOM_H_

#include "Editogia/Entity/Org/city.h"
#include "Editogia/Render/View/color.h"
#include "Editogia/Entity/race.h"
#include "world.h"
#include <vector>
#include <string>
#include <istream>

// This is the radius around a city which we lay claim to.
#define KINGDOM_CLAIM_RADIUS 8
// This is how many "points" we have to expand; adding a city generally costs 10
#define KINGDOM_EXPANSION_POINTS 200

class Kingdom
{
public:
	Kingdom();

	~Kingdom();

	void set_game(Game* g);

	std::string save_data();

	bool load_data(std::istream& data);

// Building the kingdom
// called once for each kingdom; it places its capital, the epicenter of the
// territory.
	bool place_capital(World* world, int radius = KINGDOM_CLAIM_RADIUS);

// Places a duchy city and claims territory around it.  Decreases
// expansion_points.
	bool place_duchy_seat(World* world, int& expansion_points);

// Places several minor cities around a duchy seat.  They appear within <radius>
// tiles of the duchy seat.
	void place_minor_cities(World* world, int radius = KINGDOM_CLAIM_RADIUS);

	void build_road(World* world, City* start, City* end);

	void expand_boundaries(World* world);

	void setup_trade_routes(int base_percent);  // param is for progress bar

// Data
	int uid;
	Race race;
	nc_color color;

	City* capital;
	std::vector<City*> dukes;
	std::vector<City*> cities;

// Kingdom boundaries
	int most_west, most_north, most_east, most_south;

private:
	Point pick_best_point(World* world, std::vector<Point> points_to_try,
			int radius = KINGDOM_CLAIM_RADIUS);

	void add_city(World* world, Point loc, City_type type,
			int radius = KINGDOM_CLAIM_RADIUS);

	void claim_territory(World* world, Point p);

// Data
	Game* game;

	std::vector<Point> city_locations;
};

#endif
