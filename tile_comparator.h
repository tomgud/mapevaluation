#ifndef __search_tile_comparator_h__
#define __search_tile_comparator_h__

#include "search_tile.h"

class SearchTileComparator {
    public:
        SearchTileComparator(const bool& rev=false);
        bool operator() (const SearchTile *lhs, const SearchTile *rhs) const;
    private:
        bool reverse;
};

#endif

