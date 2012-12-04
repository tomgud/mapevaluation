#include "resource_pos.h"
#include "resource.h"
#include "coords.h"

ResourcePos::ResourcePos(Resource::ResourceName name, CoOrd c) :
    name(name), c(c) { }
    
ResourcePos::ResourcePos(int res_id, CoOrd c) : name(Resource::ResourceName::NO_RES_NAME),  c(c) {
    this->name = Resource::get_name((unsigned int)res_id);
}

