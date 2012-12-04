/*
 * A class that inherites from Tile
 * Represents Snow 
 *
 * Copyright 2012 Tomas Gudmundsson
 */
#ifndef __tundra__tile__
#define __tundra__tile__

#include "Tiles/tile.h"

class Tundra : public Tile {
    protected:
    public:
        virtual ~Tundra();
        Tundra(int x, int y);
        virtual bool set_resource(const Resource &res);
        virtual bool set_feature(const Feature &feat);
        virtual Resource& get_rand_resource(const Resource::ResourceType type,
                int rnum) const;
        virtual Feature& get_rand_feature(int rnum) const;
};

#endif
