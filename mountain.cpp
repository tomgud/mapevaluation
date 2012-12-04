#include "mountain.h"
#include "tile.h"
#include "yield.h"
#include "feature.h"

Mountain::~Mountain() {
	
}

Mountain::Mountain(int x, int  y) : 
	Tile(x, y, 0, 0, 1, true, false, 0.0,
		Yield(0, 0, 0),  "Mountain")
{
	this->features_on = new Feature[8];
	this->features_on[0] = Feature(Feature::BarringerCrater);
	this->features_on[1] = Feature(Feature::MtFuji);
	this->features_on[2] = Feature(Feature::GrandMesa);
	this->features_on[3] = Feature(Feature::RockGibraltar);
	this->features_on[4] = Feature(Feature::OldFaithful);
	this->features_on[5] = Feature(Feature::FountainOfYouth);
	this->features_on[6] = Feature(Feature::CorroDePotosi);
	this->features_on[7] = Feature(Feature::ElDorado);
}

bool Mountain::set_resource(Resource &res ) {
	return false;
}

bool Mountain::set_feature(Feature &feat) {
	this->feat = feat;
	return false;
}

Resource& Mountain::get_rand_resource(Resource::ResourceType type, int rnum) const {
    return *(new Resource(Resource::NO_RES_NAME));
}

Feature& Mountain::get_rand_feature(int rnum) const {
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
    } else if (rnum % 8 == 7) {
		return *(new Feature(Feature::ElDorado));
    } else {
    	return *(new Feature(Feature::NO_FEAT));
    }
}

