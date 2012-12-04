/*
 * Map class. A representation used in this project for 
 * a Civilziation V map. Also used for evaluation, evolution
 * and generation.
 * Copyright 2012 Tomas Gudmundsson
 */

#ifndef __civ__map__shared__
#define __civ__map__shared__

#include <vector>
#include <functional>
#include <memory>
#include "Tiles/tile.h"
#include "Core/player.h"
#include "Map/coords.h"
class Genome;
class PlayerManager;
using std::shared_ptr;
using std::function;
using std::vector;
class Map {
    public:
        Map(int x = 5, int y = 20);
        ~Map();
        Map(Genome* geno, int x = 5, int y = 20);
        Map(Genome*  geno, PlayerManager*, int x = 5, int y = 20);
        Map(const Map&);
        Map& operator=(const Map&);
        shared_ptr<Tile> at(int x, int y) const;
        shared_ptr<Tile> at(CoOrd c) const;
        void set_at(int x, int y, shared_ptr<Tile> tile);
        void set_at(CoOrd c, shared_ptr<Tile> tile);
        int get_dimx() const { return this->_DIMX; }
        int get_dimy() const { return this->_DIMY; }
        void update_geno(Genome* g);
        void update_neighbours(int x, int y);
        void update_map();
        void print();
        void apply_rules();
        void loop_through(function<void(shared_ptr<Tile>)>) const;
        void loop_through_limit(function<shared_ptr<Tile> (shared_ptr<Tile>)>,
                int start_x, int start_y, int width, int height);
        shared_ptr<Tile> search_circular(function<bool (shared_ptr<Tile>)>
                func, shared_ptr<Tile> start);
        bool is_valid(const CoOrd&) const;

    private:
        int _DIMX;
        int _DIMY;
        vector<shared_ptr<Tile>> map;
        void rule_oceans();
};

#endif
