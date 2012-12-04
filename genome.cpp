#include <iostream>
#include <set>
#include <ctime>
#include <boost/random.hpp>
#include "genome.h"
#include "mtrand.h"
#include "map.h" 
#include "tile_block.h"
#include "resource_pos.h"
#include "coords.h"

Genome::Genome() : seed(0), size(0), tiles(0), resources(0), features(0), 
    singleton_features({}),  players_start(0)
{
	
}

Genome::Genome(const Genome& lval) : seed(lval.seed), size(lval.size),
    tiles(lval.tiles), resources(lval.resources), features(lval.features),
    singleton_features(lval.singleton_features), 
    players_start(lval.players_start)
{

}

Genome& Genome::operator=(const Genome& lval) 
{
    this->seed = lval.seed;
    this->size = lval.size;
    this->tiles = lval.tiles;
    this->resources = lval.resources;
    this->features = lval.features;
    this->singleton_features = lval.singleton_features;
    this->players_start = lval.players_start;
    return *this;
}

Genome::Genome(unsigned int seed, int size, int players) :
    seed(seed), size(size), tiles(size), resources(8*size), features(2*size),
    singleton_features({}), players_start(players)
{
    boost::mt19937 rng(seed);
    boost::random::uniform_int_distribution<> y_gen(0, Map::_DIMY-1);
    boost::random::uniform_int_distribution<> x_gen(0, Map::_DIMX-1);
    boost::random::uniform_int_distribution<> tile_gen(0, TileBlock::Patterns::MAX_PATTERNS);
    boost::random::uniform_int_distribution<> w_gen(4, Map::_DIMX/4);
    boost::random::uniform_int_distribution<> h_gen(4, Map::_DIMY/2);
    boost::random::uniform_int_distribution<> res_gen(0, 28);
    boost::random::uniform_int_distribution<> feat_gen(0, Feature::FeatureName::NO_FEAT);

    int c_pat = 0, c_hei = 0, c_wid = 0;
    CoOrd c_coords;

    // TileBlocks
    for (unsigned int i = 0; i < tiles.size(); ++i) {
        c_pat = tile_gen(rng);
        c_coords = CoOrd(x_gen(rng), y_gen(rng));
        c_hei = h_gen(rng);
        c_wid = w_gen(rng);
        tiles[i] = TileBlockPos(c_pat, c_coords, c_hei, c_wid); 
    }

    // Resources
    Resource::ResourceName n;
    for (unsigned int i = 0; i < resources.size(); ++i) {
        n = Resource::get_name(res_gen(rng)); 
        c_coords = CoOrd(x_gen(rng), y_gen(rng));
        resources[i] = ResourcePos(n, c_coords);
    }

    // Featuers
    Feature::FeatureName fn;
    for (unsigned int i = 0; i < features.size(); ++i) {
        fn = Feature::FeatureName(feat_gen(rng)); 
        while (Feature::is_singleton(fn) && singleton_features.count(fn) > 0) {
            fn = Feature::FeatureName((fn+1)%Feature::FeatureName::NO_FEAT);
        } 
        if (Feature::is_singleton(fn)) {
            singleton_features.insert(fn);
        }
        c_coords = CoOrd(x_gen(rng), y_gen(rng));
        features[i] = FeaturePos(fn, c_coords);
    }
    
    // Players 
    for (int i = 0; i < players; ++i) {
        players_start[i] = CoOrd(x_gen(rng), y_gen(rng));;
    }
}

void Genome::print(bool html) {
	using std::cout;
	using std::endl;
    cout << "TILES [ "; 
    for (unsigned int i = 0; i < tiles.size(); ++i) {
        cout << tiles[i].get_pat();
        cout << " (" << tiles[i].get_coord().get_x();
        cout << ", " << tiles[i].get_coord().get_y();
        cout << ") ";
    }
    cout << "]";
    if (html) cout << "<br />"; else cout << endl;
    cout << "RESOURCES [ "; 
    for (unsigned int i = 0; i < resources.size(); ++i) {
        cout << resources[i].get_res();
        cout << " (" << resources[i].get_coord().get_x();
        cout << ", " << resources[i].get_coord().get_y();
        cout << ") ";
    }
    cout << "]";
    if (html) cout << "<br />"; else cout << endl;
    
    cout << "FEATURES [ "; 
    for (unsigned int i = 0; i < features.size(); ++i) {
        cout << features[i].get_feat();
        cout << " (" << features[i].get_coord().get_x();
        cout << ", " << features[i].get_coord().get_y();
        cout << ") ";
    }
    cout << "]";
    if (html) cout << "<br />"; else cout << endl;

    cout << "PLAYERS STARTING POS [ "; 
    for (unsigned int i = 0; i < players_start.size(); ++i) {
        cout << " (" << players_start[i].get_x();
        cout << ", " << players_start[i].get_y();
        cout << ") ";
    }
    cout << "]";
    if (html) cout << "<br />"; else cout << endl;
}

void Genome::modify(int offset) {
    // Draw from an exponential distribution
    // which ones to modify
    // and then modify it from a gaussian distribution.
    boost::mt19937 rng(time(0) + offset*42);
    boost::variate_generator <boost::mt19937&, boost::normal_distribution<>> rnd(rng, boost::normal_distribution<>(0, 4));
    boost::variate_generator <boost::mt19937&, boost::normal_distribution<>> rnd_y(rng, boost::normal_distribution<>(0, Map::_DIMY/10.0));
    boost::variate_generator <boost::mt19937&, boost::normal_distribution<>> rnd_x(rng, boost::normal_distribution<>(0, Map::_DIMX/10.0));

    int cx,cy,ch,cw;
    TileBlockPos tbp;
    ResourcePos rp;
    FeaturePos fp;
    CoOrd ps, ri;
    // Random number from an exponential distribution.
    // i.e. how many times we will flip values  


    // Tiles
    for (unsigned int i = 0; i < this->tiles.size(); ++i) {
        // x,y
        cx = tiles[i].get_coord().get_x() + (int)rnd_x();
        cy = tiles[i].get_coord().get_y() + (int)rnd_y();
        cx = (cx + Map::_DIMX) % Map::_DIMX; // x wraps
        cy = (cy < 0) ? 0 : cy; // Don't want a negative index
        cy = (cy > Map::_DIMY) ? Map::_DIMY : cy; // Dont want to go out of the map
        tiles[i].get_coord().set_x_y(cx, cy);

        // Width changes
        ch = tiles[i].get_height() + (int)rnd();
        cw = tiles[i].get_width() + (int)rnd();
        ch = (ch < 2)  ? 2  : ch;
        ch = (ch > 18) ? 18 : ch;
        cw = (cw < 2)  ? 2  : cw;
        cw = (cw > 18) ? 18 : cw;
        tiles[i].set_h_w(ch, cw);

        // Pattern
        tiles[i].add_pattern((int)rnd());
    }
    
    // Resources
    for (unsigned int i = 0; i < resources.size(); ++i) {
        cx = resources[i].get_coord().get_x() + (int)rnd_x();
        cy = resources[i].get_coord().get_y() + (int)rnd_y();
        cx = (cx + Map::_DIMX) % Map::_DIMX; // x wraps
        cy = (cy < 0) ? 0 : cy; // Don't want a negative index
        cy = (cy > Map::_DIMY) ? Map::_DIMY : cy; // Dont want to go out of the map
        resources[i].get_coord().set_x_y(cx, cy);
        resources[i].add_res((int)rnd());
    }

    // Features
    Feature::FeatureName fn;
    for (unsigned int i = 0; i < features.size(); ++i) {
        fn = Feature::FeatureName::NO_FEAT;
        if (Feature::is_singleton(features[i].get_feat())) {
            singleton_features.erase(features[i].get_feat());
        }
        Feature::FeatureName fn = Feature::FeatureName(
                (features[i].get_feat() + (int)rnd()) % Feature::FeatureName::NO_FEAT);
        while (Feature::is_singleton(fn) && singleton_features.count(fn) > 0) {
            fn = Feature::FeatureName(
                (features[i].get_feat() + (int)rnd()) % Feature::FeatureName::NO_FEAT);
        }
        cx = features[i].get_coord().get_x() + (int)rnd_x();
        cy = features[i].get_coord().get_y() + (int)rnd_y();
        cx = (cx + Map::_DIMX) % Map::_DIMX; // x wraps
        cy = (cy < 0) ? 0 : cy; // Don't want a negative index
        cy = (cy > Map::_DIMY) ? Map::_DIMY : cy; // Dont want to go out of the map
        features[i].get_coord().set_x_y(cx, cy);
        features[i].set_feature(fn);
    }
    // Players
    for (unsigned int i = 0; i < players_start.size(); ++i) {
        cx = players_start[i].get_x() + (int)rnd_x();
        cy = players_start[i].get_y() + (int)rnd_y();
        cx = (cx + Map::_DIMX) % Map::_DIMX; // x wraps
        cy = (cy < 0) ? 0 : cy; // Don't want a negative index
        cy = (cy > Map::_DIMY) ? Map::_DIMY : cy; // Dont want to go out of the map
        players_start[i].set_x_y(cx, cy);
        
    }

}

vector<TileBlockPos> Genome::get_tiles() const {return this->tiles;}
vector<ResourcePos> Genome::get_resources() const {return this->resources;}
vector<FeaturePos> Genome::get_features() const {return this->features;}
vector<CoOrd> Genome::get_player_starts() const {return this->players_start;}


unsigned int Genome::get_seed() const { return this->seed; }
int Genome::get_size() const { return this->size; } 

void Genome::set_player_starts(vector<CoOrd> coords) {
    this->players_start.clear();
    this->players_start.insert(this->players_start.begin(), coords.begin(), coords.end());
}
void Genome::set_resources(vector<ResourcePos> res){
    this->resources.clear();
    this->resources.insert(this->resources.begin(), res.begin(), res.end());
}