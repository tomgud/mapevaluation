#ifndef __tundra__tile__
#define __tundra__tile__

#include "tile.h"
#include <string>

class Tundra : public Tile {
protected:
public:
	virtual ~Tundra();
	Tundra(int x, int y);
	virtual bool set_resource(Resource &res);
	virtual bool set_feature(Feature &feat);
    virtual Resource& get_rand_resource(Resource::ResourceType type, int rnum) const;
    virtual Feature& get_rand_feature(int rnum) const;
	

};

#endif
