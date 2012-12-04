/*
 * An implementation of the astar search
 * for the civilization game.
 * Copyright 2012 Tomas Gudmundsson
 *
 */
#ifndef __a_star_h__
#define __a_star_h__

#include <set>
#include <queue>
#include <vector>
#include "Tiles/tile.h"
#include "Search/tile_comparator.h"
#include "Search/search_tile.h"
#include "Map/map.h"

using std::set;
using std::vector;
using std::priority_queue;

class AStar {
    public:
        AStar();
        AStar(const Tile&, const Tile&,  const Map*);
        AStar(const AStar&);
        AStar& operator=(const AStar&);
        bool search();
        vector<SearchTile*> get_path() const;
        void allow_embark(bool embark);
        void allow_mountains(bool mountains);
        bool is_finished() const { return this->finished; }
        bool has_reached_target() const { return this->reached_target; }
        SearchTile* get_last_pos() const { return this->last_st; }
    private:
        void populate_queue(SearchTile* start);
        bool success_test(SearchTile* test);
        bool finished, reached_target, embark, mountains;
        SearchTile* last_st;
        vector<SearchTile*> visited_tiles;  // Closed list
        vector<SearchTile*> next_tiles;  // Open list
        const Tile* start;
        const Tile* end;
        const Map* map;
};
#endif

