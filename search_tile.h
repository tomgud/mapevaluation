#ifndef __search_tile_h__
#define __search_tile_h__

#include "coords.h"

class SearchTile {
    private:
        SearchTile* parent; // Where we came from
        int g_cost;         // cost so far
        int f_cost;         // estimated cost to reach goal
        CoOrd c;            // coordinates for this tile
        int age;            // Number of parents. 
                            // Works as a length of path to result
    public:
        SearchTile();
        SearchTile(SearchTile*, int, int, CoOrd);
        SearchTile(const SearchTile&);
        SearchTile& operator=(const SearchTile&);
        int get_fcost() const { return this->f_cost; }
        int get_gcost() const { return this->g_cost; }
        int get_age() const { return this->age; }
        CoOrd get_coord() const { return this->c; }
        SearchTile* get_parent() const { return this->parent; }
};

#endif
