/*
 * A genome used for representing
 * a map that can be furthered in 
 * search
 *
 * Copyright 2012 Tomas Gudmundsson
 */
#ifndef __genome__
#define __genome__
#include <vector>
#include <set>
#include <string>
#include "Map/tile_block.h"
#include "Map/resource_pos.h"
#include "Map/feature_pos.h"
using std::vector;
using std::string;
using std::set;
// Not including this definition, since tileblockpos
// includes map.h, which in turn includes this file
// creating a circular include reference which is not
// allowed.


// Possibly include a "check if ok" method
// that checks if there are too big patches
// of mountains (greater than 2x2 or 3x3)
// and no overlying patches that paint over those mountains.
// Possibly also, make sure that player starting pos
// is not in the ocean.
// Maybe also rivers, but that might be too much.
class Genome {
    public:
        Genome();
        Genome(unsigned int seed, int size, int players);
        Genome(const Genome&);
        Genome& operator=(const Genome&);
        void modify(int offset);
        void print(bool html = false);
        vector<TileBlockPos> get_tiles() const;
        vector<ResourcePos> get_resources() const;
        vector<FeaturePos> get_features() const;
        vector<CoOrd> get_player_starts() const;
        void set_player_starts(vector<CoOrd>);
        void set_resources(vector<ResourcePos>);
        unsigned int get_seed() const;
        int get_size() const;
        unsigned int get_players_count() const {
            return this->players_start.size();
        }

    private:
        // Genome setup is like so
        // size should be map width / 10
        // and map height / 8 so, 60x48 > 10x6
        // size*Triplets containing tileblock id, x,y
        // 3*size*triplets containing resource id, x,y
        // size*triplets containing feature id, x,y
        // and something about rivers
        // maybe player starting points
        unsigned int seed, size;
        vector<TileBlockPos> tiles;
        vector<ResourcePos> resources;
        vector<FeaturePos> features;
        std::set<Feature::FeatureName> singleton_features;
        vector<CoOrd> players_start;
};

#endif
