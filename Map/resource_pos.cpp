/*
 * Resource position a wrapper for 
 * a location of a resource on the map.
 * Copyright 2012 Tomas Gudmundsson
 */
#include "Map/resource_pos.h"
#include "Map/resource.h"
#include "Map/coords.h"

ResourcePos::ResourcePos(Resource::ResourceName name, CoOrd c)
    : name(name), c(c) { }
ResourcePos::ResourcePos(int res_id, CoOrd c)
    : name(Resource::ResourceName::NO_RES_NAME), c(c) {
    this->name = Resource::get_name((unsigned int)res_id);
}

