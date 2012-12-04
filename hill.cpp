#include "hill.h"

Hill::~Hill() {
    delete[] this->features_on;
    delete[] this->resources_on;
}

Hill::Hill(int x, int  y) : 
	Tile(x, y, 0, 1, 2, false, false, 0.0, 
		Yield(0, 0, 0), "Hill")
{
    // TODO : Check if forrest can be on top of a hill ?
    this->features_on = new Feature[8];
    this->features_on[0] = Feature(Feature::BarringerCrater);
    this->features_on[1] = Feature(Feature::MtFuji);
    this->features_on[2] = Feature(Feature::GrandMesa);
    this->features_on[3] = Feature(Feature::RockGibraltar);
    this->features_on[4] = Feature(Feature::OldFaithful);
    this->features_on[5] = Feature(Feature::FountainOfYouth);
    this->features_on[6] = Feature(Feature::CorroDePotosi);
    this->features_on[7] = Feature(Feature::ElDorado);

	this->resources_on = new Resource[11];
	this->resources_on[0] = Resource(Resource::Sheep);
    this->resources_on[1] = Resource(Resource::Iron);
    this->resources_on[2] = Resource(Resource::Coal);
    this->resources_on[3] = Resource(Resource::Aluminum);
    this->resources_on[4] = Resource(Resource::Uranium);
    this->resources_on[5] = Resource(Resource::Deer);
    this->resources_on[6] = Resource(Resource::Stone);
    this->resources_on[7] = Resource(Resource::Gold);
    this->resources_on[8] = Resource(Resource::Silver);
    this->resources_on[9] = Resource(Resource::Gems);
    this->resources_on[10] = Resource(Resource::Marble);

}

bool Hill::set_resource(Resource &res ) {
    this->res = res;
	return false;
}

bool Hill::set_feature(Feature &feat) {
	this->feat = feat;
	return true;
}

Resource& Hill::get_rand_resource(Resource::ResourceType type, int rnum) const {
    switch (type) {
        case Resource::Bonus:
            if (rnum % 3 == 0) {
                return *(new Resource(Resource::Sheep));
            } else if (rnum % 3 == 1) {
                return *(new Resource(Resource::Deer));
            } else {
                return *(new Resource(Resource::Stone));
            }
        case Resource::Strategic:
            if (rnum % 4 == 0) {
                return *(new Resource(Resource::Iron));
            } else if (rnum % 4 == 1) {
                return *(new Resource(Resource::Aluminum));
            } else if (rnum % 4 == 1) {
                return *(new Resource(Resource::Uranium));
            } else {
                return *(new Resource(Resource::Coal));
            }
        case Resource::Luxury:
            if (rnum % 4 == 0) {
                return *(new Resource(Resource::Gold));
            } else if (rnum % 4 == 1) {
                return *(new Resource(Resource::Silver));
            } else if (rnum % 4 == 2) {
                return *(new Resource(Resource::Gems));
            } else {
                return *(new Resource(Resource::Marble));
            }
        case Resource::NO_RES_TYPE:
        default:
            return *(new Resource(Resource::NO_RES_NAME));

    }
}

Feature& Hill::get_rand_feature(int rnum) const {
    if (rnum % 8 == 0) {
        return *(new Feature(Feature::BarringerCrater));
    } else if (rnum % 8 == 1) {
        return *(new Feature(Feature::MtFuji));
    } else if (rnum % 8 == 2) {
        return *(new Feature(Feature::GrandMesa));
    } else if (rnum % 8 == 3) {
        return *(new Feature(Feature::RockGibraltar));
    } else if (rnum % 8 == 4) {
        return *(new Feature(Feature::OldFaithful));
    } else if (rnum % 8 == 5) {
        return *(new Feature(Feature::FountainOfYouth));
    } else if (rnum % 8 == 6) {
        return *(new Feature(Feature::CorroDePotosi));
    } else {
        return *(new Feature(Feature::ElDorado));
    }
}

