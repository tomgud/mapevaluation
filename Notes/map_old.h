#ifndef __civ__map__
#define __civ__map__

#include "tile.h"
#include "player.h"
#include "coords.h"
#include <vector>
#include <functional>
class Genome;
class PlayerManager;
class Map {
public:
	Map();
	~Map();
	Map(Genome& geno);
	Map(Genome& geno, PlayerManager&);
    Map(const Map&);
    Map& operator=(const Map&);
	Tile* at(int x, int y) const;
    Tile* at(CoOrd c) const;
    Tile* operator[](CoOrd c);
	void set_at(int x, int y, Tile* tile);
	void update_neighbours(int x, int y);
	void update_map();
	void update_geno(Genome& geno);
	void apply_rules();
	void loop_through(std::function<void(Tile*)>) const;
    void loop_through_limit(std::function<Tile* (Tile*)>, int start_x, int start_y, int width, int height);
    Tile* search_circular(std::function<bool (Tile*)> func, Tile* start); 
	static const int _DIMY = 48;
	static const int _DIMX = 60;
private:
	Tile* map[_DIMY][_DIMX];
	// Rules
	void rule_oceans();
};

#endif
