/*
 * A tile, a single unit in the 
 * civilization map. Types of tiles
 * inherit from this abstract class
 *
 * Copyright 2012 Tomas Gudmundsson
 */
#include "Tiles/tile.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include "Map/map.h"

Tile::Tile()
    : owner(), x(0), y(0), feat_count(0), res_count(0),
    movement_cost(0), height(0), coord(), impassable(false),
    embarkable(false), combat_modifier(0.0), yield(0, 0, 0),
    name("NO_NAME_SET"), features_on(), resources_on(), feat(),
    res(), nei_west(), nei_nwest(), nei_swest(), nei_east(),
    nei_seast(), nei_neast(), all_nei(0) {
    this->init_neighbours();
}

Tile::Tile(const Tile& c)
    : owner(c.owner),
    x(c.x), y(c.y), feat_count(c.feat_count), res_count(c.res_count),
    movement_cost(c.movement_cost), height(height), coord(c.coord),
    impassable(c.impassable), embarkable(c.embarkable),
    combat_modifier(c.combat_modifier),  yield(c.yield), name(c.name),
    features_on(c.features_on), resources_on(c.resources_on),
    feat(c.feat), res(c.res), nei_west(c.nei_west), nei_nwest(c.nei_nwest),
    nei_swest(c.nei_swest), nei_east(c.nei_east), nei_seast(c.nei_seast),
    nei_neast(c.nei_neast), all_nei(0) {
    this->init_neighbours();
}
// TODO(tomgud): check if when added and neighbours get instantiated later
// to see if they've changed
Tile::Tile(int x, int y, int feat_count, int res_count, int mcost,
        bool impass, bool embar, float com_mod,
        Yield yield, string name)
    : owner(),
    x(x), y(y), feat_count(feat_count), res_count(res_count),
    movement_cost(mcost), height(0), coord(x, y),
    impassable(impass), embarkable(embar),
    combat_modifier(com_mod),  yield(yield), name(name),
    features_on(), resources_on(), feat(), res(),
    nei_west(), nei_nwest(), nei_swest(), nei_east(),
    nei_seast(), nei_neast(), all_nei(0) {
    this->init_neighbours();
}

void Tile::init_neighbours() {
    // Index rules for directions :
    // All x
    // north = y - 2,x
    // northwest = (y%2==0) ? y-1, x-1  : y-1, x
    // southwest = (y%2==0) ? y+1, x-1  : y+1, x
    // south = y + 2,x
    // southeast = (y%2==0) ? y+1, x    : y+1, x+1
    // northeast = (y%2==0) ? y-1, x    : y-1, x+1
    if (y == 0) {
        // No North East neighbours
        // No North West neighbours
        // nei_swest = CoOrd((((x-1)%(Map::_DIMX))), (y+1));
        nei_swest = CoOrd((((x-1))), (y+1));
        nei_seast = CoOrd((x), (y+1));
    // } else if (y == Map::_DIMY - 1) {
    } else if (y == 20 - 1) {
        // No South East neighbours
        // No South West neighbours
        if (y % 2 == 0) {
            // nei_nwest = CoOrd((((x-1)%(Map::_DIMX))), (y-1));
            nei_nwest = CoOrd((((x-1))), (y-1));
            nei_neast = CoOrd((x), (y-1));
        } else {
            nei_nwest = CoOrd((x), (y-1));
            // nei_neast = CoOrd((((x+1)%(Map::_DIMX))), (y-1));
            nei_neast = CoOrd((((x+1))), (y-1));
        }
    } else {
        // All neighbours exist
        if (y % 2 == 0) {
            // nei_nwest = CoOrd((((x-1)%(Map::_DIMX))), (y-1));
            // nei_swest = CoOrd((((x-1)%(Map::_DIMX))), (y+1));
            nei_nwest = CoOrd((((x-1))), (y-1));
            nei_swest = CoOrd((((x-1))), (y+1));
            nei_seast = CoOrd((x), (y+1));
            nei_neast = CoOrd((x), (y-1));
        } else {
            nei_nwest = CoOrd((x), (y-1));
            nei_swest = CoOrd((x), (y+1));
            nei_seast = CoOrd((((x+1))), (y+1));
            nei_neast = CoOrd((((x+1))), (y-1));
            // nei_seast = CoOrd((((x+1)%(Map::_DIMX))), (y+1));
            // nei_neast = CoOrd((((x+1)%(Map::_DIMX))), (y-1));
        }
    }
    // Everyone has EAST and WEST neighbours
    // nei_west = CoOrd((x-1+Map::_DIMX)%(Map::_DIMX), y);
    nei_west = CoOrd((x-1), y);
    // nei_east = CoOrd((x+1)%(Map::_DIMX), y);
    nei_east = CoOrd((x+1), y);
}

Tile::~Tile() {
}

Tile& Tile::operator=(Tile const& ref) {
    if (this == &ref) return *this;
    this->set_owner(ref.get_owner());
    return *this;
}

CoOrd Tile::get_neighbour(TileDir dir) const {
    switch (dir) {
        case HEX_WEST:
            return this->nei_west;
        case HEX_NWEST:
            return this->nei_nwest;
        case HEX_SWEST:
            return this->nei_swest;
        case HEX_EAST:
            return this->nei_east;
        case HEX_SEAST:
            return this->nei_seast;
        case HEX_NEAST:
            return this->nei_neast;
        default :
            return CoOrd();
    }
}

vector<CoOrd> Tile::get_all_neighbours() {
    if (this->all_nei.size() != 0)
        return this->all_nei;
    if (this->nei_west.get_x() != CoOrd::oob
            && this->nei_west.get_y() != CoOrd::oob)
        this->all_nei.push_back(this->nei_west);
    if (this->nei_nwest.get_x() != CoOrd::oob
            && this->nei_nwest.get_y() != CoOrd::oob)
        this->all_nei.push_back(this->nei_nwest);
    if (this->nei_swest.get_x() != CoOrd::oob
            && this->nei_swest.get_y() != CoOrd::oob)
        this->all_nei.push_back(this->nei_swest);
    if (this->nei_east.get_x() != CoOrd::oob
            && this->nei_east.get_y() != CoOrd::oob)
        this->all_nei.push_back(this->nei_east);
    if (this->nei_seast.get_x() != CoOrd::oob
            && this->nei_seast.get_y() != CoOrd::oob)
        this->all_nei.push_back(this->nei_seast);
    if (this->nei_neast.get_x() != CoOrd::oob
            && this->nei_neast.get_y() != CoOrd::oob)
        this->all_nei.push_back(this->nei_neast);
    return this->all_nei;
}

bool Tile::is_embarkable() const {
    return this->embarkable;
}

bool Tile::is_impassable() const {
    if (this->feat.get_name() != Feature::NO_FEAT)
        return this->feat.is_impassable() || this->impassable;
    return this->impassable;
}

string Tile::get_name() const {
    return this->name;
}

int Tile::get_movement_cost() const {
    return this->movement_cost;
}
int Tile::get_combat_modifier() const {
    return this->combat_modifier;
}

bool Tile::is_allowed(const Feature &feat) const {
    for (int i = 0; i < feat_count; ++i) {
        if (features_on[i].get_name() == feat.get_name())
            return true;
    }
    return false;
}

bool Tile::is_allowed(const Feature::FeatureName &fn) const {
    for (int i = 0; i < feat_count; ++i) {
        if (features_on[i].get_name() == fn)
            return true;
    }
    return false;
}

bool Tile::is_allowed(const Resource &res) const {
    for (int i = 0; i < res_count; ++i) {
        if (resources_on[i].get_name() == res.get_name())
            return true;
    }
    return false;
}

bool Tile::is_allowed(const Resource::ResourceName &rn) const {
    for (int i = 0; i < res_count; ++i) {
        if (resources_on[i].get_name() == rn)
            return true;
    }
    return false;
}

string Tile::print() const {
    string val = string(this->name.substr(0, 2));
    if (this->feat.get_name() != Feature::NO_FEAT) {
        // Feature encountered.
        return this->feat.get_string().substr(0, 4) + " ";
    }
    if (this->res.get_name() != Resource::NO_RES_NAME) {
        val += this->res.get_string().substr(0, 2);
    }
    return val + " ";
}

string Tile::print_html() const {
    string val = "";
    val += this->name;
    if (this->feat.get_name() != Feature::NO_FEAT) {
        // Feature encountered.
        val += " " + this->feat.get_string();
    }
    if (this->res.get_name() != Resource::NO_RES_NAME) {
        val += " " + this->res.get_string();
    }
    return val;
}

void Tile::set_owner(const Player& p) {
    this->owner = p;
}

bool Tile::set_feature(const Feature &feat) {
    // Abstract method
    return false;
}

bool Tile::set_resource(const Resource &res) {
    return false;
}

Resource& Tile::get_rand_resource(Resource::ResourceType type, int rnum) const {
    // Abstract method
    return *(new Resource(Resource::NO_RES_NAME));
}

Yield Tile::get_yield() const {
    Yield c_y = this->yield;
    if (this->feat.get_name() != Feature::NO_FEAT) {
        // we have a feature on this tile
        if (this->feat.is_impassable())
            return this->feat.get_yield();
        else
            c_y = c_y + this->feat.get_yield();
    }
    if (this->res.get_name() != Resource::NO_RES_NAME) {
        c_y = c_y + this->res.get_yield();
    }
    return c_y;
}

void Tile::set_height(int h) {
    // Height can be 0 = flat lands
    // or 1 = hills
    // or 2 = mountains
    if (h >= 0 && h < 3) {
        this->height = h;
    }
}
