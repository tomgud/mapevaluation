#include "map.h"
#include <vector>
#include <cstdlib>
#include <iostream>
#include <functional>
#include <queue>
#include "resource.h"
#include "coast.h"
#include "desert.h"
#include "grassland.h"
#include "hill.h"
#include "mountain.h"
#include "ocean.h"
#include "plains.h"
#include "snow.h"
#include "tundra.h"
#include "tile_block.h"
#include "genome.h"
#include "player_manager.h"


Map::Map() {
    // for (int y = 0; y < _DIMY; ++y) {
    //     for (int x = 0; x < _DIMX; ++x) {
    //         map[y][x] = new Ocean(x,y);
    //     }
    // }
}

Map::Map(const Map& rval) {
    for (int y = 0; y < _DIMY; ++y) {
        for (int x = 0; x < _DIMX; ++x) {
            map[y][x] = new Tile(*rval.at(x,y));
        }
    }
}

Map& Map::operator=(const Map& rval) {
    for (int y = 0; y < _DIMY; ++y) {
        for (int x = 0; x < _DIMX; ++x) {
            map[y][x] = new Tile(*rval.at(x,y));
        }
    }
    return *this;
}

Map::~Map() {
    for (int y = 0; y < _DIMY; ++y) {
        for (int x = 0; x < _DIMX; ++x) {
            delete map[y][x];
        }
    }
}

Map::Map(Genome& geno) {
    // This really should never be called. Think of removing! ???
	for (int y = 0; y < _DIMY; ++y) {
        for (int x = 0; x < _DIMX; ++x) {
            map[y][x] = new Ocean(x,y);
        }
    }
    this->update_geno(geno);
}

Map::Map(Genome& geno, PlayerManager& pm)  {
    for (int y = 0; y < _DIMY; ++y) {
        for (int x = 0; x < _DIMX; ++x) {
            map[y][x] = new Ocean(x,y);
        }
    }
    this->update_geno(geno);
    vector<CoOrd> player_starts = geno.get_player_starts();
    for (unsigned int pl(0); pl < player_starts.size(); ++pl) {
        pm.set_coord(pl, player_starts[pl]);
    }
}

void Map::update_geno(Genome& geno) {
    using std::cout;
    using std::endl;
    vector<TileBlockPos> tile_blocks = geno.get_tiles();
    for (unsigned int i = 0; i < tile_blocks.size(); ++i) {
        TileBlock::write(*this, tile_blocks[i].get_pat(), 
                tile_blocks[i].get_coord().get_x(),
                tile_blocks[i].get_coord().get_y(),
                tile_blocks[i].get_height(),
                tile_blocks[i].get_width()); 
    }
    this->update_map();

    vector<ResourcePos> resources = geno.get_resources(); 
    Resource::ResourceName rn;
    for (unsigned int i = 0; i < resources.size(); ++i) {
        rn = resources[i].get_res();
        Tile* result;
        result = search_circular([&] (Tile* t) {
            int nei_res = 0;
            vector<CoOrd> neis = t->get_all_neighbours();
            for (unsigned int nei(0); nei < neis.size(); ++nei) {
                if ((this->at(neis[nei])->get_resource().get_name() != Resource::NO_RES_NAME)) 
                    nei_res++;
            }
            return (t->get_resource().get_name() == Resource::NO_RES_NAME && t->is_allowed(rn) && (nei_res < 1));
        }, this->at(resources[i].get_coord()));
        if (result->is_allowed(rn)) {
            Resource n = Resource(rn);
            result->set_resource(n);
            resources[i].set_coord(result->get_coord());
        }

    }
    geno.set_resources(resources);

    vector<FeaturePos> features = geno.get_features();
    Feature::FeatureName fn;
    for (unsigned int i = 0; i < features.size(); ++i) {
        fn = features[i].get_feat();
        Tile* result;
        result = search_circular([&] (Tile* t) {
            return (t->get_feature().get_name() != Feature::NO_FEAT && t->is_allowed(fn));
        }, this->at(features[i].get_coord()));
        if (result->is_allowed(fn)) {
            Feature n = Feature(fn);
            result->set_feature(n);
            features[i].set_coord(result->get_coord());
        }
    }

    vector<CoOrd> player_starts = geno.get_player_starts();
    Tile* cur_pl_st;
    for (unsigned int cur_pl(0); cur_pl < player_starts.size(); ++cur_pl) {
        cur_pl_st = this->at(player_starts[cur_pl]);
        cur_pl_st = search_circular([&] (Tile* t) {
            // TODO reason why you should do this: "Make sure that flight of the bird length is less than N"
            return (!t->is_impassable() && !t->is_embarkable());
        }, cur_pl_st);
        player_starts[cur_pl] = cur_pl_st->get_coord();
    }
    geno.set_player_starts(player_starts);
}

void Map::update_map() {
    this->apply_rules();
}



Tile* Map::at(int x, int y) const {
	return map[y][x];
}

Tile* Map::operator[](CoOrd c) {
    return map[c.get_y()][c.get_x()];
}

Tile* Map::at(CoOrd c) const {
    return map[c.get_y()][c.get_x()];
}

void Map::set_at(int x, int y, Tile* tile) {
	map[y][x] = tile;
}

void Map::apply_rules() {
	this->rule_oceans();
}

void Map::rule_oceans() {
    vector<CoOrd> cur_neighbours;
    for (int y = 0; y < _DIMY; ++y) {
        for (int x = 0; x < _DIMX; x++) {
            cur_neighbours = map[y][x]->get_all_neighbours();
            if (map[y][x]->get_name() == "Ocean") {
                for (unsigned int i = 0; i < cur_neighbours.size(); ++i) {
                    // if (cur_neighbours[i].get_x() == CoOrd::oob ||
                    //     cur_neighbours[i].get_y() == CoOrd::oob) {
                        // We don't want to get here. 
                        // continue;
                    // }
                    // We are at an ocean, so all non ocean neighbours
                    // must become coasts
                    if (this->at(cur_neighbours[i])->get_name() != "Ocean") {
                        int cur_x = (cur_neighbours[i].get_x() + Map::_DIMX) % Map::_DIMX;
                        int cur_y = (cur_neighbours[i].get_y() + Map::_DIMY) % Map::_DIMY;
                        Coast* cur_coast = new Coast(cur_x, cur_y);
                        delete map[cur_y][cur_x];
                        map[cur_y][cur_x] = cur_coast;
                    }
                } 

            } else {
                for (unsigned int i = 0; i < cur_neighbours.size(); ++i) {
                    // if (cur_neighbours[i].get_x() == CoOrd::oob ||
                        // cur_neighbours[i].get_y() == CoOrd::oob) {
                        // We don't want to get here. 
                        // continue;
                    // }
                    // We are not at an ocean, so all ocean neigbours
                    // must become coasts
                    if (this->at(cur_neighbours[i])->get_name() == "Ocean") {
                        Coast* cur_coast = new Coast(x, y);
                        delete map[y][x];
                        map[y][x] = cur_coast;
                    }
                }
            }
        }
    }
}

void Map::loop_through(std::function<void(Tile*)> func) const {
	for (int y = 0; y < _DIMY; ++y) {
		for (int x = 0; x < _DIMX; ++x) {
			func(map[y][x]);
		}
	}
}

void Map::loop_through_limit(std::function<Tile* (Tile*)> func, int start_x, int start_y, int width, int height) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Tile* test;
            test = func(map[(start_y+y)%_DIMY][(start_x+x)%_DIMX]);
            if (test != 0x0) {
                // delete map[(start_y+y)%_DIMY][(start_x+x)%_DIMX];
                map[(start_y+y)%_DIMY][(start_x+x)%_DIMX] = test;
            }
        }
    }
}


/* 
  Super useful search. Goes through all neighbours in a circular
  fashion to search for the goal. Goal test is expressed in the
  lambda function func. An example of finding a spot which is totally 
  surrounded by mountains:
    Map map_from_file;
    Tile* result(map_from_file.at(14,20));
    result = map_from_file.search_circular([&] (Tile* t) {
        bool val(true);
        vector<CoOrd> neis = t->get_all_neighbours();
        for (unsigned int nei(0); nei < neis.size(); ++nei) {
            if (neis[nei].get_x() != CoOrd::oob)
                val &= (map_from_file.at(neis[nei])->get_name() == "Mountain");
        }
        val &= (t->get_name() == "Mountain");
        return val;
    }, result);
    return 0;
 */
Tile* Map::search_circular(std::function<bool (Tile*)> func, Tile* start) 
{
    std::queue<Tile*> next_tiles;
    std::set<CoOrd> next_coords;
    std::set<CoOrd> visited_coords;
    Tile* current = start;
    next_tiles.push(current);
    vector<CoOrd> neighbours;
    if (start == 0x0 || current == 0x0) { cout << "FUU"; return start;}
    // While we haven't reach our end criteria and we still have stuff to look at
    while(!next_tiles.empty() && !func(current)) {
        // Now we have visited it
        visited_coords.insert(current->get_coord());
        // Look at all its neighbours, see if there is someone we haven't been to
        // and look at those next.
        neighbours = current->get_all_neighbours();
        for (unsigned int nei_ind = 0; nei_ind < neighbours.size(); ++nei_ind) 
        {
            if (visited_coords.count(neighbours[nei_ind]) == 0 &&
                next_coords.count(neighbours[nei_ind]) == 0 ) { 
                next_tiles.push(this->at(neighbours[nei_ind].get_x(),neighbours[nei_ind].get_y()));
                next_coords.insert(neighbours[nei_ind]);
            }
        }
        // Removing this tile from next tiles to visit queue
        next_tiles.pop();
        current = next_tiles.front();
    }
    if (!next_tiles.empty() && func(current))
        return current;
    else 
        return start;
}
