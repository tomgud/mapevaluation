/*
 * A class that inherites from Tile
 * Represents Ocean 
 *
 * Copyright 2012 Tomas Gudmundsson
 */
#include "Tiles/ocean.h"
#include "Tiles/tile.h"
#include "Map/yield.h"
#include "Map/feature.h"

Ocean::~Ocean() {
    delete[] this->features_on;
}

Ocean::Ocean(int x, int  y)
    : Tile(x, y, 1, 0, 1, false, true, 0.0,
        Yield(1, 0, 1), "Ocean") {
    this->features_on = new Feature[3];
    // Setting up available features
    this->features_on[0] = Feature(Feature::Ice);
    this->features_on[1] = Feature(Feature::Atoll);
    this->features_on[2] = Feature(Feature::GreatBarrierReef);
}

bool Ocean::set_resource(const Resource &res ) {
    this->res= res;
    return false;
}

bool Ocean::set_feature(const Feature &feat) {
    this->feat = feat;
    return true;
}

Resource& Ocean::get_rand_resource(const Resource::ResourceType type,
        int rnum) const {
    return *(new Resource(Resource::NO_RES_NAME));
}

Feature& Ocean::get_rand_feature(int rnum) const {
    if (rnum % 3 == 0) {
        return *(new Feature(Feature::Ice));
    } else if (rnum % 3 == 1) {
        return *(new Feature(Feature::Atoll));
    } else {
        return *(new Feature(Feature::GreatBarrierReef));
    }
}

