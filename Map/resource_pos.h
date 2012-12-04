/*
 * Resource position a wrapper for 
 * a location of a resource on the map.
 * Copyright 2012 Tomas Gudmundsson
 */
#ifndef __resource_pos_h__
#define __resource_pos_h__

#include "Map/resource.h"
#include "Map/coords.h"

class ResourcePos {
    private:
        Resource::ResourceName name;
        CoOrd c;
    public:
        ResourcePos(Resource::ResourceName =
                Resource::ResourceName::NO_RES_NAME, CoOrd = CoOrd());
        ResourcePos(int res_id, CoOrd = CoOrd());
        CoOrd get_coord() const { return this->c; }
        void set_coord(CoOrd c) { this->c = c; }
        Resource::ResourceName get_res() const { return this->name; }
        void add_res(int val) {
            this->name = Resource::get_name(this->name + val);
        }
};

#endif
