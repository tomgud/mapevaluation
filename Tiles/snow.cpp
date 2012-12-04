/*
 * A class that inherites from Tile
 * Represents Snow 
 *
 * Copyright 2012 Tomas Gudmundsson
 */
#include "Tiles/snow.h"
#include "Tiles/tile.h"
#include "Map/yield.h"
#include "Map/feature.h"

Snow::~Snow() {
    delete[] this->resources_on;
    delete this->features_on;
}

Snow::Snow(int x, int  y)
    : Tile(x, y, 0, 3, 1, false, false, 0.0,
        Yield(0, 0, 0), "Snow") {
    this->features_on = new Feature(Feature::RockGibraltar);
    this->resources_on = new Resource[4];
    this->resources_on[0] = Resource(Resource::Iron);
    this->resources_on[1] = Resource(Resource::Oil);
    this->resources_on[2] = Resource(Resource::Uranium);
    this->resources_on[3] = Resource(Resource::Stone);
}

bool Snow::set_resource(const Resource &res ) {
    this->res = res;
    return true;
}

bool Snow::set_feature(const Feature &feat) {
    this->feat = feat;
    return true;
}

Resource& Snow::get_rand_resource(const Resource::ResourceType type,
        int rnum) const {
    switch (type) {
        case Resource::Bonus:
            return *(new Resource(Resource::Stone));
        case Resource::Strategic:
            if (rnum % 3 == 0) {
                return *(new Resource(Resource::Iron));
            } else if (rnum % 3 == 1) {
                return *(new Resource(Resource::Oil));
            } else {
                return *(new Resource(Resource::Uranium));
            }
        case Resource::Luxury:
        case Resource::NO_RES_TYPE:
        default:
            return *(new Resource(Resource::NO_RES_NAME));
    }
}


Feature& Snow::get_rand_feature(int rnum) const {
    return *(new Feature(Feature::RockGibraltar));
}
