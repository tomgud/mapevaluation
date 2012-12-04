#include "tundra.h"
#include "tile.h"
#include "yield.h"
#include "feature.h"

Tundra::~Tundra() {
    delete[] this->features_on;
    delete[] this->resources_on;
}

Tundra::Tundra(int x, int  y) : 
	Tile(x, y, 1, 10, 1, false, false, 0.0, 
		Yield(1, 0, 0), "Tundra")
		
{
	this->features_on = new Feature[9];
	this->resources_on = new Resource[11];
	
	// Setting up available features
	this->features_on[0] = Feature(Feature::Forest);
    this->features_on[1] = Feature(Feature::BarringerCrater);
    this->features_on[2] = Feature(Feature::MtFuji);
    this->features_on[3] = Feature(Feature::GrandMesa);
    this->features_on[4] = Feature(Feature::RockGibraltar);
    this->features_on[5] = Feature(Feature::OldFaithful);
    this->features_on[6] = Feature(Feature::FountainOfYouth);
    this->features_on[7] = Feature(Feature::CorroDePotosi);
    this->features_on[8] = Feature(Feature::ElDorado);	
	// Setting up available resources
	this->resources_on[0] = Resource(Resource::Iron);
	this->resources_on[1] = Resource(Resource::Oil);
	this->resources_on[2] = Resource(Resource::Aluminum);
	this->resources_on[3] = Resource(Resource::Uranium);
	this->resources_on[4] = Resource(Resource::Horses);
	this->resources_on[5] = Resource(Resource::Deer);
	this->resources_on[6] = Resource(Resource::Silver);
	this->resources_on[7] = Resource(Resource::Gems);
	this->resources_on[8] = Resource(Resource::Marble);
	this->resources_on[9] = Resource(Resource::Furs);
    this->resources_on[10] = Resource(Resource::Stone);
	
}

bool Tundra::set_resource(Resource &res ) {
    this->res = res;
	return false;
}

bool Tundra::set_feature(Feature &feat) {
	this->feat = feat;
	return true;
}

Resource& Tundra::get_rand_resource(Resource::ResourceType type, int rnum) const {
    switch (type) {
        case Resource::Bonus:
            return *(new Resource(Resource::Deer));
        case Resource::Strategic:
            if (rnum % 5 == 0) {
                return *(new Resource(Resource::Iron)); 
            } else if (rnum % 5 == 1) {
                return *(new Resource(Resource::Oil));
            } else if (rnum % 5 == 2) {
                return *(new Resource(Resource::Aluminum));
            } else if (rnum % 5 == 3) {
                return *(new Resource(Resource::Uranium));
            } else {
                return *(new Resource(Resource::Horses));
            }
        case Resource::Luxury:
            if (rnum % 4 == 0) {
                return *(new Resource(Resource::Silver));
            } else if (rnum % 4 == 1) {
                return *(new Resource(Resource::Gems));
            } else if (rnum % 4 == 2) {
                return *(new Resource(Resource::Marble));
            } else {
                return *(new Resource(Resource::Furs));
            }
        case Resource::NO_RES_TYPE:
        default:
            return *(new Resource(Resource::NO_RES_NAME));

    }
}


Feature& Tundra::get_rand_feature(int rnum) const {
    if (rnum % 9 == 0) {
        return *(new Feature(Feature::Forest));
    } else if (rnum % 9 == 1) {
        return *(new Feature(Feature::BarringerCrater));
    } else if (rnum % 9 == 2) {
        return *(new Feature(Feature::MtFuji));
    } else if (rnum % 9 == 3) {
        return *(new Feature(Feature::GrandMesa));
    } else if (rnum % 9 == 4) {
        return *(new Feature(Feature::RockGibraltar));
    } else if (rnum % 9 == 5) {
        return *(new Feature(Feature::OldFaithful));
    } else if (rnum % 9 == 6) {
        return *(new Feature(Feature::FountainOfYouth));
    } else if (rnum % 9 == 7) {
        return *(new Feature(Feature::CorroDePotosi));
    } else if (rnum % 9 == 8) {
        return *(new Feature(Feature::ElDorado));
    } else {
        return *(new Feature(Feature::NO_FEAT));
    }
}
