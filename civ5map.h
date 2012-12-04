#ifndef __civ5map_h__
#define __civ5map_h__
#include <string>
#include <vector>
#include "map.h"
#include "tile.h"
// This class is based on this reference :
// http://forums.civfanatics.com/showthread.php?t=418566

class Civ5Map {

private:
	// char type_version;
	int map_width;
	int map_height;
	// char no_players_scenario;
	int misc_settings;
	vector<string> terrain_types;
	vector<string> feature_types;
	vector<string> resource_types;
	Tile*** map;
	bool init;
	void init_map();
	/*
byte 0 -- Terrain type ID (index into list of Terrain types read from header)
byte 1 -- Resource type ID; 0xFF if none
byte 2 -- 1st Feature type ID; 0xFF if none
byte 3 -- River indicator (non-zero if tile borders a river; actual value probably indicates direction)
byte 4 -- Elevation (0 = Flat, 1 = Hill, 2 = Mountain)
byte 5 -- Unknown (possibly related to continent art style)
byte 6 -- 2nd Feature type ID; 0xFF if none
byte 7 -- Unknown
*/

public:
	Civ5Map();
    Civ5Map(const Civ5Map&);
    Civ5Map& operator=(const Civ5Map&);
	void to_map(Map& container);
	// Setters
	void set_map_width(int val);
	void set_map_height(int val);
	void set_misc_settings(int val);
	void add_terrain_type(string val);
	void add_feature_type(string val);
	void add_resource_type(string val);
	void set_map(int x, int y, Tile* val);
	// Getters
	int get_map_width() const;
	int get_map_height() const;
	string get_terrain_types(int index) const;
	string get_feature_types(int index) const;
	string get_resource_types(int index) const;
	unsigned int get_terrain_types_length() const;
	unsigned int get_feature_types_length() const;
	unsigned int get_resource_types_length() const;
};

#endif
