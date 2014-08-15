#ifndef _CITY_H_
#define _CITY_H_

#include "citizen.h"
#include "building.h"
#include "area.h"
#include "map.h"
#include "window.h"
#include "cuss.h"
#include <map>

struct Citizens
{
  int count;
  int wealth;

  Citizens() { count = 0; wealth = 0; }
};

class City
{
public:
  City();
  ~City();

  Citizens population[CIT_MAX];
  //int population[CIT_MAX];
  int open_buildings[BUILD_MAX];
  int closed_buildings[BUILD_MAX];
  int resources[RES_MAX];

  int radius;

  std::vector<Area> areas;
  std::vector<Area> area_queue;
  int labor_pool;

  City_map map;

  bool place_keep();

  void interface_buildings();
  void draw_map(cuss::element* e_draw, Point sel, bool radius_limited = false);
  void display_map(Window* w, cuss::interface* i_map, bool interactive = false,
                   bool radius_limited = false);

  void do_turn();
  void add_area_to_queue(Area_type type, Point location);
  void add_area_to_queue(Area area);

  int get_total_population();

  bool expend_resources(std::vector<Resource_amount> resources);
  bool expend_resources(std::map<Resource,int> resources);
};

#endif
