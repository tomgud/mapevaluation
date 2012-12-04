/*
 * Fitness functions that 
 * grade a map and position of players
 *
 * Copyright 2012 Tomas Gudmundsson
 *
 */
#ifndef __fitness_h__
#define __fitness_h__
#include "Map/map.h"
#include "Core/player_manager.h"

class Fitness {
    public :
        static void assign_resources(Map*,
                const PlayerManager&, bool);  // f_a
        static void reset_resources(Map* m);
        // enemies
        static double starting_positions(const Map &,
                const PlayerManager&);  // f1
        // teammates
        static double starting_positions_teammates(const Map&,
                const PlayerManager&);  // f2
        // food_per_playerN / total_food
        // prod_per_playerN / total_prod
        // gold_per_playerN / total_gold
        static double throughput_per_player(const Map&,
                const PlayerManager&);  // f3
        // unique_resource_count_per_player / unique_resources
        static double unique_resources(const Map&, const PlayerManager&);  // f4
        // strategic_resources_count_per_player / total_strategic_resources
        static double strategic_resource(const Map&,
                const PlayerManager&);  // f5
        // can reach all players without embarking from every other player
        static double players_in_a_continent(const Map&,
                const PlayerManager&);  // f6
        // eliminator
        static double total_throughput(const Map&);  // f_e
};

#endif
