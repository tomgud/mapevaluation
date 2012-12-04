/*
 * A class that inherites from Tile
 * Represents Coast
 *
 * Copyright 2012 Tomas Gudmundsson
 */
#include "Tiles/coast.h"

Coast::~Coast() {
    delete[] this->features_on;
    delete[] this->resources_on;
}

Coast::Coast(int x, int  y)
    : Tile(x, y, 1, 4, 1, false, true, 0.0,
        Yield(1, 0, 1), "Coast") {
    this->features_on = new Feature[3];
    this->resources_on = new Resource[4];

    // Setting up available feature
    this->features_on[0] = Feature(Feature::Ice);
    this->features_on[1] = Feature(Feature::Atoll);
    this->features_on[2] = Feature(Feature::GreatBarrierReef);

    // Setting up available resources
    this->resources_on[0] = Resource(Resource::Oil);
    this->resources_on[1] = Resource(Resource::Fish);
    this->resources_on[2] = Resource(Resource::Whales);
    this->resources_on[3] = Resource(Resource::Pearls);
}

bool Coast::set_resource(const Resource &res ) {
    this->res = res;
    return true;
}

bool Coast::set_feature(const Feature &feat) {
    this->feat = feat;
    return true;
}

Feature& Coast::get_rand_feature(int rnum) const {
    if (rnum % 3 == 0) {
        return *(new Feature(Feature::Ice));
    } else if (rnum % 3 == 1) {
        return *(new Feature(Feature::Atoll));
    } else if (rnum % 3 == 2) {
        return *(new Feature(Feature::GreatBarrierReef));
    } else {
        return *(new Feature(Feature::NO_FEAT));
    }
}

Resource& Coast::get_rand_resource(Resource::ResourceType type,
        int rnum) const {
    switch (type) {
        case Resource::Bonus:
            return *(new Resource(Resource::Fish));
        case Resource::Strategic:
            return *(new Resource(Resource::Oil));
        case Resource::Luxury:
            if (rnum % 2 == 0) {
                return *(new Resource(Resource::Whales));
            } else {
                return *(new Resource(Resource::Pearls));
            }
        case Resource::NO_RES_TYPE:
        default:
            return *(new Resource(Resource::NO_RES_NAME));
    }
}
