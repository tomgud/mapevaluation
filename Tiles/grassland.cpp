/*
 * A class that inherites from Tile
 * Represents Grassland 
 *
 * Copyright 2012 Tomas Gudmundsson
 */
#include "Tiles/grassland.h"

Grassland::~Grassland() {
    delete[] this->features_on;
    delete[] this->resources_on;
}

Grassland::Grassland(int x, int  y)
    : Tile(x, y, 2, 11, 1, false, false, 0.0,
        Yield(2, 0, 0), "Grassland") {
    this->features_on = new Feature[10];
    this->resources_on = new Resource[12];

    // Setting up available features
    this->features_on[0] = Feature(Feature::Marsh);
    this->features_on[1] = Feature(Feature::Forest);
    this->features_on[2] = Feature(Feature::BarringerCrater);
    this->features_on[3] = Feature(Feature::MtFuji);
    this->features_on[4] = Feature(Feature::GrandMesa);
    this->features_on[5] = Feature(Feature::RockGibraltar);
    this->features_on[6] = Feature(Feature::OldFaithful);
    this->features_on[7] = Feature(Feature::FountainOfYouth);
    this->features_on[8] = Feature(Feature::CorroDePotosi);
    this->features_on[9] = Feature(Feature::ElDorado);

    // Setting up available resources
    this->resources_on[0] = Resource(Resource::Iron);
    this->resources_on[1] = Resource(Resource::Horses);
    this->resources_on[2] = Resource(Resource::Coal);
    this->resources_on[3] = Resource(Resource::Uranium);
    this->resources_on[4] = Resource(Resource::Sheep);
    this->resources_on[5] = Resource(Resource::Gold);
    this->resources_on[6] = Resource(Resource::Cattle);
    this->resources_on[7] = Resource(Resource::Gems);
    this->resources_on[8] = Resource(Resource::Marble);
    this->resources_on[9] = Resource(Resource::Cotton);
    this->resources_on[10] = Resource(Resource::Wine);
    this->resources_on[11] = Resource(Resource::Stone);
}

bool Grassland::set_resource(const Resource &res ) {
    this->res = res;
    return false;
}

bool Grassland::set_feature(const Feature &feat) {
    this->feat = feat;
    return true;
}

Resource& Grassland::get_rand_resource(const Resource::ResourceType type,
        int rnum) const {
    switch (type) {
        case Resource::Bonus:
            return *(new Resource(Resource::Sheep));
        case Resource::Strategic:
            if (rnum % 5 == 0) {
                return *(new Resource(Resource::Iron));
            } else if (rnum % 5 == 1) {
                return *(new Resource(Resource::Horses));
            } else if (rnum % 5 == 2) {
                return *(new Resource(Resource::Coal));
            } else if (rnum % 5 == 3) {
                return *(new Resource(Resource::Stone));
            } else {
                return *(new Resource(Resource::Uranium));
            }
        case Resource::Luxury:
            if (rnum % 6 == 0) {
                return *(new Resource(Resource::Gold));
            } else if (rnum % 6 == 1) {
                return *(new Resource(Resource::Cattle));
            } else if (rnum % 6 == 2) {
                return *(new Resource(Resource::Gems));
            } else if (rnum % 6 == 3) {
                return *(new Resource(Resource::Marble));
            } else if (rnum % 6 == 4) {
                return *(new Resource(Resource::Cotton));
            } else {
                return *(new Resource(Resource::Wine));
            }
        case Resource::NO_RES_TYPE:
        default:
            return *(new Resource(Resource::NO_RES_NAME));
    }
}

Feature& Grassland::get_rand_feature(int rnum) const {
    if (rnum % 10 == 0) {
        return *(new Feature(Feature::Marsh));
    } else if (rnum % 10 == 1) {
        return *(new Feature(Feature::Forest));
    } else if (rnum % 10 == 2) {
        return *(new Feature(Feature::BarringerCrater));
    } else if (rnum % 10 == 3) {
        return *(new Feature(Feature::MtFuji));
    } else if (rnum % 10 == 4) {
        return *(new Feature(Feature::GrandMesa));
    } else if (rnum % 10 == 5) {
        return *(new Feature(Feature::RockGibraltar));
    } else if (rnum % 10 == 6) {
        return *(new Feature(Feature::OldFaithful));
    } else if (rnum % 10 == 7) {
        return *(new Feature(Feature::FountainOfYouth));
    } else if (rnum % 10 == 8) {
        return *(new Feature(Feature::CorroDePotosi));
    } else if (rnum % 10 == 9) {
        return *(new Feature(Feature::ElDorado));
    } else {
        return *(new Feature(Feature::NO_FEAT));
    }
}
