/*
 * A tile, a single unit in the 
 * civilization map. Types of tiles
 * inherit from this abstract class
 *
 * Copyright 2012 Tomas Gudmundsson
 */
#ifndef __i__tile__
#define __i__tile__
#include <string>
#include <vector>
#include "Core/player.h"
#include "Map/feature.h"
#include "Map/yield.h"
#include "Map/resource.h"
#include "Map/coords.h"


using std::string;
using std::vector;

class Tile {
    public:
        Tile();
        Tile(int x, int y, int feat_count, int res_count, int mcost,
            bool impass, bool embar, float com_mod,
            Yield yield, string name);
        virtual ~Tile();
        Tile(const Tile&);
        Tile& operator=(Tile const&);
        enum TileDir {
            HEX_NWEST,
            HEX_WEST,
            HEX_SWEST,
            HEX_SEAST,
            HEX_EAST,
            HEX_NEAST
        };
        virtual bool set_resource(const Resource &res);
        virtual bool set_feature(const Feature &feat);
        virtual void set_owner(const Player& p);
        virtual void set_height(int h);
        virtual int get_x() const { return this->x;}
        virtual int get_y() const { return this->y;}
        virtual CoOrd get_coord() const { return this->coord; }
        virtual CoOrd get_neighbour(TileDir dir) const;
        virtual vector<CoOrd> get_all_neighbours();
        virtual int get_movement_cost() const;
        virtual int get_combat_modifier()  const;
        virtual string get_name() const;
        virtual bool is_impassable() const;
        virtual bool is_embarkable() const;
        virtual bool is_allowed(const Resource&) const;
        virtual bool is_allowed(const Resource::ResourceName&) const;
        virtual bool is_allowed(const Feature&) const;
        virtual bool is_allowed(const Feature::FeatureName&) const;
        virtual Player get_owner() const {return this->owner;}
        virtual Resource& get_rand_resource(Resource::ResourceType type,
                int rnum) const;
        virtual Resource get_resource() const {return this->res;}
        virtual Yield get_yield() const;
        virtual Feature get_feature() const {return this->feat;}
        virtual string print() const;
        virtual string print_html() const;
        virtual void reset_owner() { owner = Player(); }

    protected:
        void init_neighbours();
        Player owner;
        int x, y, feat_count, res_count, movement_cost, height;
        CoOrd coord;
        bool impassable, embarkable;
        float combat_modifier;
        Yield yield;
        string name;
        Feature *features_on;
        Resource *resources_on;
        Feature feat;
        Resource res;
        CoOrd nei_west, nei_nwest, nei_swest, nei_east, nei_seast, nei_neast;
        vector<CoOrd> all_nei;
};

#endif
