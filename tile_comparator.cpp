#include "tile_comparator.h"
#include "search_tile.h"

SearchTileComparator::SearchTileComparator(const bool& rev) : reverse(rev) {};
bool SearchTileComparator::operator() (const SearchTile *lhs, const SearchTile *rhs) const 
{
    return lhs->get_fcost() > rhs->get_fcost();
}
