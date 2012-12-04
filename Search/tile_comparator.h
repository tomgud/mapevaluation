/*
 * A comparator class for A* search of Search tiles.
 * Copyright 2012 Tomas Gudmundsson
 */
#ifndef __search_tile_comparator_h__
#define __search_tile_comparator_h__

#include "Search/search_tile.h"

class SearchTileComparator {
    public:
        explicit SearchTileComparator(const bool& rev = false);
        bool operator() (const SearchTile *lhs, const SearchTile *rhs) const;

    private:
        bool reverse;
};

#endif

