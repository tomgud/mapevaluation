/*
 * A comparator class for A* search of Search tiles.
 * Copyright 2012 Tomas Gudmundsson
 */
#include "Search/tile_comparator.h"
#include "Search/search_tile.h"

SearchTileComparator::SearchTileComparator(const bool& rev) : reverse(rev) {}
bool SearchTileComparator::operator()
    (const SearchTile *lhs, const SearchTile *rhs) const {
    return lhs->get_fcost() > rhs->get_fcost();
}
