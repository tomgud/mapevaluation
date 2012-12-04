/*
 * Map class. A representation used in this project for 
 * a Civilziation V map. Also used for evaluation, evolution
 * and generation.
 * Copyright 2012 Tomas Gudmundsson
 */

#include <vector>
#include <cstdlib>
#include <iostream>
#include <functional>
#include <queue>
#include <set>
#include "Map/map.h"
#include "Map/resource.h"
#include "Tiles/coast.h"
#include "Tiles/desert.h"
#include "Tiles/grassland.h"
#include "Tiles/hill.h"
#include "Tiles/mountain.h"
#include "Tiles/ocean.h"
#include "Tiles/plains.h"
#include "Tiles/snow.h"
#include "Tiles/tundra.h"
#include "Map/tile_block.h"
#include "Evolution/genome.h"
#include "Core/player_manager.h"

Map::Map(int x, int y) : _DIMX(x), _DIMY(y),
map() {
    for (int i = 0; i < _DIMY; ++i) {
        for (int j = 0; j < _DIMX; ++j) {
            map.push_back(std::shared_ptr<Tile>(new Ocean(j, i)));
        }
    }
}

Map::~Map() {}

Map::Map(const Map& rval)
    : _DIMX(rval._DIMX),
    _DIMY(rval._DIMY),
    map() {
    map.clear();
    for (int i = 0; i < _DIMY; ++i) {
        for (int j = 0; j < _DIMX; ++j) {
            this->map.push_back(shared_ptr<Tile>(new Tile(*rval.at(j, i))));
        }
    }
}

Map& Map::operator=(const Map& rval) {
    this->map.clear();
    this->_DIMX = rval._DIMX;
    this->_DIMY = rval._DIMY;
    for (int i = 0; i < _DIMY; ++i) {
        for (int j = 0; j < _DIMX; ++j) {
            this->map.push_back(shared_ptr<Tile>(new Tile(*rval.at(j, i))));
        }
    }
    return *this;
}

Map::Map(Genome* geno, int x, int y)
    : _DIMX(x),
    _DIMY(y),
    map() {
    for (int i = 0; i < _DIMY; ++i) {
        for (int j = 0; j < _DIMX; ++j) {
            map.push_back(std::shared_ptr<Tile>(new Ocean(j, i)));
        }
    }
    this->update_geno(geno);
}

Map::Map(Genome* geno, PlayerManager* pm, int x, int y)
    : _DIMX(x),
    _DIMY(y),
    map() {
    for (int i = 0; i < _DIMY; ++i) {
        for (int j = 0; j < _DIMX; ++j) {
            map.push_back(std::shared_ptr<Tile>(new Ocean(j, i)));
        }
    }

    this->update_geno(geno);

    vector<CoOrd> player_starts = geno->get_player_starts();
    for (unsigned int pl(0); pl < player_starts.size(); ++pl) {
        pm->set_coord(pl, player_starts[pl]);
    }
}

void Map::update_geno(Genome* geno) {
    using std::cout;
    using std::endl;
    vector<TileBlockPos> tile_blocks = geno->get_tiles();
    for (unsigned int i = 0; i < tile_blocks.size(); ++i) {
        TileBlock::write(this, tile_blocks[i].get_pat(),
                tile_blocks[i].get_coord().get_x(),
                tile_blocks[i].get_coord().get_y(),
                tile_blocks[i].get_height(),
                tile_blocks[i].get_width());
    }
    this->update_map();
    
    std::cout << "1";
    vector<ResourcePos> resources = geno->get_resources();
    Resource::ResourceName rn;
    for (unsigned int i = 0; i < resources.size(); ++i) {
        rn = resources[i].get_res();
        std::shared_ptr<Tile> result;
        result = search_circular([&] (std::shared_ptr<Tile> t) {
            int nei_res = 0;
            vector<CoOrd> neis = t.get()->get_all_neighbours();
            for (unsigned int nei(0); nei < neis.size(); ++nei) {
                if (!is_valid(neis[nei])) continue;
                if ((this->at(neis[nei]).get()->get_resource().get_name()
                        != Resource::NO_RES_NAME))
                    nei_res++;
            }
            return (t.get()->get_resource().get_name() == Resource::NO_RES_NAME
                && t.get()->is_allowed(rn) && (nei_res < 1));
        }, this->at(resources[i].get_coord()));
        if (result.get()->is_allowed(rn)) {
            Resource n = Resource(rn);
            result.get()->set_resource(n);
            resources[i].set_coord(result->get_coord());
        }
    }
    geno->set_resources(resources);
    std::cout << "2";

    vector<FeaturePos> features = geno->get_features();
    Feature::FeatureName fn;
    for (unsigned int i = 0; i < features.size(); ++i) {
        fn = features[i].get_feat();
        std::shared_ptr<Tile> result;
        result = search_circular([&] (std::shared_ptr<Tile> t) {
            return (t.get()->get_feature().get_name() != Feature::NO_FEAT
                && t.get()->is_allowed(fn));
        }, this->at(features[i].get_coord()));
        if (result.get()->is_allowed(fn)) {
            Feature n = Feature(fn);
            result.get()->set_feature(n);
            features[i].set_coord(result->get_coord());
        }
    }
    std::cout << "3";

    vector<CoOrd> player_starts = geno->get_player_starts();
    std::shared_ptr<Tile> cur_pl_st;
    for (unsigned int cur_pl(0); cur_pl < player_starts.size(); ++cur_pl) {
        cur_pl_st = this->at(player_starts[cur_pl]);
        cur_pl_st = search_circular([&] (std::shared_ptr<Tile> t) {
            // TODO(tomgud):  reason why you should do this:
            // "Make sure that flight of the bird length is less than N"
            return (!t.get()->is_impassable() && !t.get()->is_embarkable());
        }, cur_pl_st);
        player_starts[cur_pl] = cur_pl_st.get()->get_coord();
    }
    geno->set_player_starts(player_starts);
}

bool Map::is_valid(const CoOrd& c) const {
    return (c.get_x() >= 0 && c.get_x() < this->_DIMX) &&
        (c.get_y() >= 0 && c.get_y() < this->_DIMY);
}

std::shared_ptr<Tile> Map::at(int x, int y) const {
    return this->map[this->_DIMX*y + x];
}

std::shared_ptr<Tile> Map::at(CoOrd c) const {
    return this->map[this->_DIMX*c.get_y() + c.get_x()];
}

void Map::set_at(int x, int y, std::shared_ptr<Tile> tile) {
    this->map[_DIMX*y + x] = tile;
}

void Map::set_at(CoOrd c, std::shared_ptr<Tile> tile) {
    this->map[_DIMX*c.get_y() + c.get_x()] = tile;
}

void Map::print() {
    for (int i = 0; i < _DIMY; ++i) {
        for (int j = 0; j < _DIMX; ++j) {
            std::cout << "[" << j << "," << i << "]: " << "( "
                << (_DIMX*i + j) << ") ";
            std::cout << this->map[_DIMX*i + j].get()->get_name() << " (";
            std::cout << this->map[_DIMX*i + j].get()->get_x() << ",";
            std::cout << this->map[_DIMX*i + j].get()->get_y() << ") "
                << std::endl;
        }
    }
}



void Map::apply_rules() {
    this->rule_oceans();
}

void Map::rule_oceans() {
    return;
    vector<CoOrd> cur_neighbours;
    for (int y = 0; y < _DIMY; ++y) {
        for (int x = 0; x < _DIMX; x++) {
            cur_neighbours = this->at(x, y).get()->get_all_neighbours();
            if (this->at(x, y).get()->get_name() == "Ocean") {
                for (unsigned int i = 0; i < cur_neighbours.size(); ++i) {
                    // if (cur_neighbours[i].get_x() == CoOrd::oob ||
                    //     cur_neighbours[i].get_y() == CoOrd::oob) {
                        // We don't want to get here.
                        // continue;
                    // }
                    // We are at an ocean, so all
                    // non ocean neighbours must become coasts
                    if (this->at(cur_neighbours[i])->get_name() != "Ocean") {
                        int cur_x = (cur_neighbours[i].get_x() + _DIMX) % _DIMX;
                        int cur_y = (cur_neighbours[i].get_y() + _DIMY) % _DIMY;
                        shared_ptr<Coast> cur_coast(new Coast(cur_x, cur_y));
                        this->set_at(cur_x, cur_y, cur_coast);
                    }
                }

            } else {
                for (unsigned int i = 0; i < cur_neighbours.size(); ++i) {
                    if (cur_neighbours[i].get_x() >= this->_DIMX||
                        cur_neighbours[i].get_y() >= this->_DIMY) {
                        // We don't want to get here.
                         continue;
                    }
                    // We are not at an ocean, so all ocean neigbours
                    // must become coasts
                    if (this->at(cur_neighbours[i]).get()->get_name()
                            == "Ocean") {
                        std::shared_ptr<Coast> cur_coast(new Coast(x, y));
                        this->set_at(x, y, cur_coast);
                    }
                }
            }
        }
    }
}

void Map::update_map() {
    this->apply_rules();
}

void Map::loop_through(std::function<void(std::shared_ptr<Tile>)> func) const {
    for (int y = 0; y < _DIMY; ++y) {
        for (int x = 0; x < _DIMX; ++x) {
            func(this->at(x, y));
        }
    }
}

void Map::loop_through_limit(function<shared_ptr<Tile> (std::shared_ptr<Tile>)>
        func, int start_x, int start_y, int width, int height) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::shared_ptr<Tile> result;
            // test = func(map[(start_y+y)%_DIMY][(start_x+x)%_DIMX]);
            result = func(this->at((start_y+y)%this->_DIMY,
                    (start_x+x) % _DIMX));
            if (result != 0x0) {
                // delete map[(start_y+y)%_DIMY][(start_x+x)%_DIMX];
                this->set_at((start_y+y)%_DIMY, (start_x+x)%_DIMX, result);
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
shared_ptr<Tile> Map::search_circular(function<bool (shared_ptr<Tile>)> func,
        std::shared_ptr<Tile> start) {
    std::queue<std::shared_ptr<Tile>> next_tiles;
    std::set<CoOrd> next_coords;
    std::set<CoOrd> visited_coords;
    std::shared_ptr<Tile> current = start;
    next_tiles.push(current);
    vector<CoOrd> neighbours;
    if (start.get() == 0x0 || current.get() == 0x0) {
        std::cout << "FUU";
        return start;
    }
    // While we haven't reach our end criteria
    // and we still have stuff to look at
    while (!next_tiles.empty() &&
            current.use_count() > 0
            && !func(current)) {
        // Now we have visited it
        visited_coords.insert(current.get()->get_coord());
        // Look at all its neighbours, see if
        // there is someone we haven't been to
        // and look at those next.
        neighbours = current.get()->get_all_neighbours();
        for (unsigned int nei_ind = 0; nei_ind < neighbours.size(); ++nei_ind) {
            if (!is_valid(neighbours[nei_ind])) {
                continue;
            }
            if (visited_coords.count(neighbours[nei_ind]) == 0 &&
                next_coords.count(neighbours[nei_ind]) == 0 ) {
                next_tiles.push(this->at(neighbours[nei_ind].get_x(),
                            neighbours[nei_ind].get_y()));
                next_coords.insert(neighbours[nei_ind]);
            }
        }
        // Removing this tile from next tiles to visit queue
        next_tiles.pop();
        if (next_tiles.size() == 0) 
            return start;
        current = next_tiles.front();
    }
    if (!next_tiles.empty() && func(current))
        return current;
    else
        return start;
}
