/*
 * Feature position a wrapper for 
 * a location of a feature on the map.
 * Copyright 2012 Tomas Gudmundsson
 */
#ifndef __feature_pos_h__
#define __feature_pos_h__

#include "Map/feature.h"
#include "Map/coords.h"

class FeaturePos {
    private:
        Feature::FeatureName name;
        CoOrd c;

    public:
        FeaturePos(Feature::FeatureName = Feature::FeatureName::NO_FEAT,
                CoOrd = CoOrd());
        FeaturePos(int feat_id, CoOrd = CoOrd());
        CoOrd get_coord() const { return this->c; }
        void set_coord(CoOrd c) { this->c = c; }
        Feature::FeatureName get_feat() const { return this->name; }
        void set_feature(Feature::FeatureName n) { this->name = n; }
};

#endif
