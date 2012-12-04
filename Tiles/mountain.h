/*
 * A class that inherites from Tile
 * Represents Mountain 
 *
 * Copyright 2012 Tomas Gudmundsson
 */
#ifndef __mountain__tile__
#define __mountain__tile__

#include "Tiles/tile.h"

class Mountain : public Tile {
    protected:
    public:
        virtual ~Mountain();
        Mountain(int x, int y);
        virtual bool set_resource(const Resource &res);
        virtual bool set_feature(const Feature &feat);
        virtual Resource& get_rand_resource(const Resource::ResourceType type,
                int rnum) const;
        virtual Feature& get_rand_feature(int rnum) const;
};

#endif
