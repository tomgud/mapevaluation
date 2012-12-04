#ifndef __fitness_h__
#define __fitness_h__
#include "map.h"
#include "player_manager.h"

class Fitness {
    public :
        static void assign_resources(Map&, PlayerManager &, bool); // f_a
        static void reset_resources(Map&);
        // enemies 
        static double starting_positions(Map &,  PlayerManager &); // f1
        // teammates 
        static double starting_positions_teammates(Map &,  PlayerManager &); // f2
        // food_per_playerN / total_food
        // prod_per_playerN / total_prod
        // gold_per_playerN / total_gold
        static double throughput_per_player(Map &,  PlayerManager &); // f3
        // unique_resource_count_per_player / unique_resources
        static double unique_resources(Map&,  PlayerManager&); //f4
        // strategic_resources_count_per_player / total_strategic_resources
        static double strategic_resource(Map&,  PlayerManager&); // f5
        // can reach all players without embarking from every other player
        static double players_in_a_continent(Map&,  PlayerManager&); //f6     
        // eliminator
        static double total_throughput(Map&); // f_e
};

#endif
