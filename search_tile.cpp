#include "search_tile.h"
#include "coords.h"

SearchTile::SearchTile() : parent(0), g_cost(0.0),
    f_cost(0.0), c(), age(0)
{
}

SearchTile::SearchTile(const SearchTile& ref) :
    parent(ref.parent), g_cost(ref.g_cost), 
    f_cost(ref.f_cost), c(ref.c), age(ref.age)
{
}

SearchTile& SearchTile::operator=(const SearchTile & rval) {
    parent = rval.parent;
    g_cost = rval.g_cost; 
    f_cost = rval.f_cost; 
    c = rval.c;
    age = rval.age;
    return *this;
}

SearchTile::SearchTile(SearchTile* p, int g, int h, CoOrd c) :
    parent(p), g_cost(g),  f_cost(0.0), c(c), age(0)
{
    if (this->parent == 0) 
    {
        this->f_cost = g+h;
    } 
    else 
    {
        this->age = p->get_age() + 1;
        this->f_cost = ((g+h) > p->get_fcost()) ? g+h : p->get_fcost();
    }
}
