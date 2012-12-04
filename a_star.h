#ifndef __a_star_h__
#define __a_star_h__

#include "tile.h"
#include "tile_comparator.h"
#include "search_tile.h"
#include "map.h"
#include <set>
#include <queue>
#include <vector>

using std::set;
using std::vector;
using std::priority_queue;

class AStar {
    public:
        AStar();
        AStar(Tile&, Tile&,  Map*);
        AStar(const AStar&);
        AStar& operator=(const AStar&);
        bool search();
        vector<SearchTile*> get_path() const;
        void allow_embark(bool);
        void allow_mountains(bool);
        bool is_finished() const { return this->finished; }
        bool has_reached_target() const { return this->reached_target; }
        SearchTile* get_last_pos() const { return this->last_st; }
    private:
        void populate_queue(SearchTile*);
        bool success_test(SearchTile*);
        bool finished, reached_target, embark, mountains;
        SearchTile* last_st;
        vector<SearchTile*> visited_tiles; // Closed list
        vector<SearchTile*> next_tiles; // Open list
        Tile* start;
        Tile* end;
        Map* map;

};
#endif

