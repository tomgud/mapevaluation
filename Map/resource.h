/*
 * Resource class representing the resources 
 * on a civilization map.
 * Copyright 2012 Tomas Gudmundsson
 *
 */
#ifndef __resource__
#define __resource__
#include <string>
#include "Map/yield.h"

using std::string;

class Resource {
    public:
        enum ResourceType {
            Bonus = 0,
            Strategic = 1,
            Luxury = 2,
            NO_RES_TYPE = 3
        };
        enum ResourceName {
            // Bonus resources
            Banana = 0,
            Cattle = 1,
            Deer = 2,
            Fish = 3,
            Sheep = 4,
            Stone = 5,
            Wheat = 6,
            // Strategic resources
            Aluminum = 100,
            Coal = 101,
            Horses = 102,
            Iron = 103,
            Oil = 104,
            Uranium = 105,
            // Luxury resources
            Cotton = 200,
            Dyes = 201,
            Furs = 202,
            Gems = 203,
            Gold = 204,
            Incense = 205,
            Ivory = 206,
            Marble = 207,
            Pearls = 208,
            Silk = 209,
            Silver = 210,
            Spices = 211,
            Sugar = 212,
            Whales = 213,
            Wine = 214,
            NO_RES_NAME = 300
        };

        // Dear reader of this header file.
        // I am aware of the horrible naming used for two of these functions
        // and I am also aware that using the SAME name for a function should
        // only be used when they actually do the same. However, one of the
        // get_name is used to return the name of the object it is used on,
        // the other, static get_name, is used to get a ResourceName from some
        // index, due to the numbering convention used for the enum ResourceName
        // note, there is also a get_index from a name and you can look at this
        // as a enum-to-int and int-to-enum functions. Sorry for the confusion.
        // Dear Tomas. You should be ashamed for such a bad design!!!
        // Also, here is code for the test to make sure
        // the conversion is working.
        /*
        include <iostream>
        for (int i = 0; i < 29; ++i) {
            Resource r(Resource::get_name(i));
            if (Resource::get_index(r.get_name()) == i)
                std::cout << "index conversion number " << i << " worked << std::endl;
            else 
                std::cout << i << " failed " << static_cast<int>(r.get_name()) << " " << r.get_string() << std::endl;
        }
        */
        ResourceName get_name() const {return this->name;}
        ResourceType get_type() const {return this->type;}
        Yield get_yield() const {return this->yield;}
        Resource();
        explicit Resource(ResourceName name);
        explicit Resource(string name);
        string get_string() const;
        static ResourceName get_name(unsigned int index);
        static int get_index(ResourceName name);

    protected:
        ResourceName name;
        ResourceType type;
        Yield yield;
        void init(ResourceName name);
};

#endif
