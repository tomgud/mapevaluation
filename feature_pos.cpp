#include "feature_pos.h"
#include "feature.h"
#include "coords.h"

FeaturePos::FeaturePos(Feature::FeatureName name, CoOrd c) :
    name(name), c(c) { }
    
FeaturePos::FeaturePos(int res_id, CoOrd c) : name(Feature::FeatureName::NO_FEAT),  c(c) {
    // if feat_id fits then ok, else not
    Feature f = Feature(Feature::FeatureName(res_id));
    if (f.get_string().size() != 0) 
        this->name = Feature::FeatureName(res_id);
}

