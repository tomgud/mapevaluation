/*
 * Civ5Map class. A representation of the
 * civ5map file format. civ5map files are
 * read and stored here intermittently 
 * until a Map/map object is created
 *
 * Copyright 2012 Tomas Gudmundsson
 */
#include "Map/civ5map.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include "Map/map.h"

Civ5Map::Civ5Map() : map_width(0), map_height(0),
    misc_settings(0),
    terrain_types(0), feature_types(0), resource_types(0),
    map(), init(false) {
}

Civ5Map::Civ5Map(const Civ5Map& c)
    : map_width(c.map_width), map_height(c.map_height),
    misc_settings(c.misc_settings), terrain_types(c.terrain_types),
    feature_types(c.feature_types), resource_types(c.resource_types),
    map(), init(false) {
    this->init_map();
    for (int i = 0; i < map_height; ++i) {
        for (int j = 0; j < map_width; ++j) {
            map[i][j] = c.map[i][j];
        }
    }
}

Civ5Map& Civ5Map::operator=(const Civ5Map& c) {
    this->map_width = c.map_width;
    this->map_height = c.map_height;
    this->misc_settings = c.misc_settings;
    this->terrain_types.empty();
    // TODO(tomgud): add the vectors correctly
    // this->terrain_types(c.terrain_types);
    // this->feature_types
    // this->resource_types
    this->init = false;
    this->init_map();
    for (int i = 0; i < map_height; ++i) {
        for (int j = 0; j < map_width; ++j) {
            map[i][j] = c.map[i][j];
        }
    }
    return *this;
}


void Civ5Map::set_map_width(int val) {
    this->map_width = val;
}
void Civ5Map::set_map_height(int val) {
    this->map_height = val;
}

void Civ5Map::set_misc_settings(int val) {
    this->misc_settings = val;
}

void Civ5Map::add_terrain_type(string val) {
    this->terrain_types.push_back(val);
}

void Civ5Map::add_feature_type(string val) {
    this->feature_types.push_back(val);
}

void Civ5Map::add_resource_type(string val) {
    this->resource_types.push_back(val);
}

void Civ5Map::init_map() {
    if (this->map_height != 0 && this->map_width != 0) {
        this->map = reinterpret_cast<Tile***>(
                calloc(this->map_height, sizeof(Tile**)));
        for (int h = 0; h < this->map_height; ++h) {
            this->map[h] = reinterpret_cast<Tile**>(
                    calloc(this->map_width, sizeof(Tile*)));
        }
    }
    this->init = true;
}

void Civ5Map::set_map(int x, int y, Tile* val) {
    if (!this->init) {
        this->init_map();
        return this->set_map(x, y, val);
    } else {
        this->map[y][x] = val;
    }
}


void Civ5Map::to_map(Map* container) {
    if (this->map_width == container->get_dimx()
            && this->map_height == container->get_dimy()) {
        for (int i = 0; i < this->map_height; ++i) {
            for (int j = 0; j < this->map_width; ++j) {
                // TODO(tomgud):  ... make all pointers in civ5map shared_ptr
                container->set_at(j, this->map_height-1-i,
                        shared_ptr<Tile>(new Tile(*this->map[i][j])));
            }
        }
    } else {
        // Error
        std::cout << this->map_width << "," << this->map_height;
    }
}

int Civ5Map::get_map_width() const { return this->map_width; }
int Civ5Map::get_map_height() const { return this->map_height; }

string Civ5Map::get_terrain_types(int index) const {
    return this->terrain_types[index];
}
string Civ5Map::get_feature_types(int index) const {
    return this->feature_types[index];
}
string Civ5Map::get_resource_types(int index) const {
    return this->resource_types[index];
}

unsigned int Civ5Map::get_terrain_types_length() const {
    return this->terrain_types.size();
}

unsigned int Civ5Map::get_feature_types_length() const {
    return this->feature_types.size();
}

unsigned int Civ5Map::get_resource_types_length() const {
    return this->resource_types.size();
}
