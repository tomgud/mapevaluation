/*
 * A class that handles
 * the evolution of the maps
 *
 * Copyright 2012 Tomas Gudmundsson
 *
 */
#ifndef __evolution_h__
#define __evolution_h__

#include <ctime>
#include <vector>
#include "Map/map.h"
#include "Core/player_manager.h"
class Evolution {
    public:
        Evolution(unsigned int seed = time(0), int iters = 100, int size = 50,
                double min_ch = 0.01, int genome_sz = 48, int no_players = 4);
        void run();
        vector<Genome> get_genomes() const { return this->gen_pop; }
    private:
        unsigned int seed;
        int iters, size, genome_size, no_of_players;
        double min_change;
        bool all_req_min_chg;
        vector<Genome> gen_pop;
        void iterate();
};


#endif
