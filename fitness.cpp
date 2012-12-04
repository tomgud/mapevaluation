#include "fitness.h"
#include "map.h"
#include "player.h"
#include "yield.h"
#include "a_star.h"
#include "tile.h"
#include "resource.h"
#include "player_manager.h"
#include <iostream>
#include <math.h>

using std::cout;
using std::endl;
    
void Fitness::assign_resources(Map& m, PlayerManager & pm, bool use_astar)
{
    AStar search;
    double min_length(Map::_DIMX*Map::_DIMY);
    int shortest_player(-1);
    int cur_length;
    for (int i = 0; i < Map::_DIMY; ++i) {
        for (int j = 0; j < Map::_DIMX; ++j) {
            if (m.at(j,i)->get_resource().get_name() != Resource::NO_RES_NAME)
            {
                min_length = Map::_DIMX*Map::_DIMY;
                shortest_player = -1;
                vector<Player> players = pm.get_players();
                cur_length = 0;
                for (unsigned int k = 0; k < players.size(); ++k) {
                    if (!use_astar) {
                    cur_length = CoOrd::get_length(CoOrd(j,i), players[k].get_start());
                    if (min_length > cur_length) {
                        min_length = cur_length;
                        shortest_player = k;
                    }
                    } else {
                    search = AStar(
                        *m.at(j,i),
                        *m.at(players[k].get_start()),
                        &m
                    );
                    search.allow_embark(true);
                    search.allow_mountains(false);    
                    search.search();
                    if (search.has_reached_target()) 
                    {
                        if (min_length > search.get_last_pos()->get_age())
                        {
                            min_length = search.get_last_pos()->get_age();
                            shortest_player = k;
                        }
                    }
                    }// end of if use_astar

                }
                if (shortest_player != -1) 
                {
                    m.at(j,i)->set_owner(players[shortest_player]);
                } else 
                {
                    cout << "Critical error. Could not find any player closest to a resource at " << j << "," << i << endl;
                }

            }   
        }
    }
}

void Fitness::reset_resources(Map& m) {
    for (int i = 0; i < Map::_DIMY; ++i) {
        for (int j = 0; j < Map::_DIMX; ++j) {
            m.at(j,i)->reset_owner();
        }
    }
}

double Fitness::starting_positions(Map &m,  PlayerManager &p) 
{
    // We only do two teams.
    vector<Player> team_a(p.get_teammembers(0));
    vector<Player> team_b(p.get_teammembers(1));
    AStar search;
    double total_length(0);
    for (unsigned int i = 0; i < team_a.size(); ++i) 
    {
        for (unsigned int j = 0; j < team_b.size(); ++j)
        {
            search = AStar(
                *m.at(team_a[i].get_start()),
                *m.at(team_b[j].get_start()),
                &(m));
            search.search();
            if (search.has_reached_target()) 
            {
                total_length += search.get_last_pos()->get_age();        
            }
        }

    }
   
    // THESIS
    // Justify this pythagoras 1
    // total_length is the sum of the lengths between all players in team a to all players in team b
    // for example, 4 players, a1_to_b1+a1_to_b2+a2_to_b1+a2_to_b2
    // then the average length between the teams is the sum divided by two 
    // ideal length is the diagonal between the square inside the map, that covers a fourth of the map
    // i.e. top left starts at (0+DIMX/4 ; 0+DIMY/4) bottom right is at (DIMX-DIMX/4, DIMY- DIMY/4)
    // so this square has one side as a = ((DIMX-DIMX/4)-(DIMX/4)) = DIMX-DIMX/2
    // and the other side is b = ((DIMY-DIMY/4)-(DIMY/4)) = DIMY-DIMY/2
    // then according to pythagoras we have diagonal as a^2 + b^2
    double fitness = total_length/(team_a.size()+team_b.size());
    double a = Map::_DIMX/2.0;
    double b = Map::_DIMY/2.0;
    fitness = fitness / sqrt((a*a + b*b));
    // Max value here is 1
    fitness = (fitness > 1.0) ? 1 - ((fitness - 1.0)/fitness) : fitness;
    return fitness;
}

double Fitness::starting_positions_teammates(Map &m, PlayerManager& pm) 
{
    if (pm.get_no_of_players() != 4) 
        return 1.0;
    
    double total_length(0.0);
    vector<Player> team_a(pm.get_teammembers(0));
    vector<Player> team_b(pm.get_teammembers(1));

    AStar search(
        *m.at(team_a[0].get_start()),
        *m.at(team_a[1].get_start()),
        &(m));
    search.allow_mountains(true);
    search.allow_embark(true);
    search.search();
    if (search.has_reached_target()) 
    {
        total_length += search.get_last_pos()->get_age();
    }

    search = AStar(
        *m.at(team_b[0].get_start()),
        *m.at(team_b[1].get_start()),
        &(m));
    search.search();
    if (search.has_reached_target()) 
    {
        total_length += search.get_last_pos()->get_age();
    }
    // average length between teammates
    total_length = total_length / 2;

    // THESIS
    // Justify a good relative length
    // ( DIMX / 4 + DIMY / 4 ) 
    total_length = total_length / (Map::_DIMX / 4 + Map::_DIMY / 4);
    total_length = (total_length > 1.0) ? 1 - ((total_length - 1.0)/total_length) : total_length;
    // Max here is 1
    return total_length;
}

double Fitness::throughput_per_player(Map& m, PlayerManager& pm) {
    // gold, food, and production per player
    double player_throughput[pm.get_no_of_players()];
    double total_throughput(0.0);
    double avg_player_throughput(0.0);

    // Going through all of the map.
    Yield c_yield;
    int c_pl(-1);
    for (int i = 0; i < Map::_DIMY; ++i) {
        for (int j = 0; j < Map::_DIMX; ++j) {
            if (m.at(j,i)->get_resource().get_name() != Resource::NO_RES_NAME) {
                c_yield = m.at(j,i)->get_yield();
                if (m.at(j,i)->get_owner().get_id() == -1) {
                    cout << "Owner of a resource is not set, did you forget to call assign_resources()? " << endl;
                    return -1.0;
                }
                c_pl = m.at(j,i)->get_owner().get_id();
                player_throughput[c_pl] += c_yield.get_sum();
                total_throughput += c_yield.get_sum();
            }
            

        }
    } // Finished counting up total_throughput, and resources per player.

    int best_count(4);
    Yield cur_best_of[best_count];
    std::set<CoOrd> all_nei;
    vector<CoOrd> rad_one_nei;
    vector<CoOrd> rad_two_nei;
    for (int pl_id(0); pl_id < pm.get_no_of_players(); ++pl_id) {
        // Set up radius one
        rad_one_nei = m.at(pm.get_player(pl_id).get_start())->get_all_neighbours();
        for (unsigned int ron_ind(0); ron_ind < rad_one_nei.size(); ++ron_ind) {
            // radius two
            rad_two_nei = m.at(rad_one_nei[ron_ind])->get_all_neighbours();
            for (unsigned int rtn_ind(0); rtn_ind < rad_two_nei.size(); ++ rtn_ind) {
                if (all_nei.count(rad_two_nei[rtn_ind]) == 0) {
                    // We have not considered this coord before, consider it considered.
                    all_nei.insert(rad_two_nei[rtn_ind]);
                    c_yield = m.at(rad_two_nei[rtn_ind])->get_yield();
                    for (int cbi(0); cbi < best_count; ++cbi) {
                        if (cur_best_of[cbi] < c_yield) {
                            cur_best_of[cbi] = c_yield;
                            break;
                        }
                    }
                }

            }
            rad_two_nei.clear();
        }
        // Resetting for this player.
        all_nei.clear();
        for (int cbi(0); cbi < best_count; ++cbi) {
            player_throughput[pl_id] += cur_best_of[cbi].get_sum();
            total_throughput += cur_best_of[cbi].get_sum();
            cur_best_of[cbi] = Yield(0, 0, 0);
        }
    }
    double cur_avg(0.0);
    for (int i = 0; i < pm.get_no_of_players(); ++i) 
    {
        cur_avg = ((player_throughput[i]/total_throughput)/(1.0/pm.get_no_of_players()));
        if (cur_avg > 1.0) {
            cur_avg = 1 - ((cur_avg - 1.0)/cur_avg);
        }
        avg_player_throughput += cur_avg; //player_throughput[i]/total_throughput;
    }

    // Max here is 1
    return avg_player_throughput / pm.get_no_of_players();
}

double Fitness::unique_resources(Map& m, PlayerManager& pm)
{
    int no_players = pm.get_no_of_players();
    int max_resources = 27;
    bool unique_found[no_players][max_resources];

    // initialize unqiue found
    for (int i = 0; i < no_players; ++i) 
        for (int j = 0; j < max_resources; ++j)
            unique_found[i][j] = false;

    // looking for uniques
    for (int i = 0; i < Map::_DIMY; ++i) {
        for (int j = 0; j < Map::_DIMX; ++j) {
            if (m.at(j,i)->get_resource().get_name() != Resource::NO_RES_NAME) {
                if (m.at(j,i)->get_owner().get_id() == -1) {
                    cout << "Owner of a resource is not set, did you forget to call assign_resources()? " << endl;
                    return -1.0;
                }
                unique_found[m.at(j,i)->get_owner().get_id()][Resource::get_index(m.at(j,i)->get_resource().get_name())] = true;
            }
        }
    }

    // Maximum value here is 4*28 = 116.
    double unique_per_player(0.0);
    double cur_unique_count(0);
    for (int i = 0; i < no_players; ++i)  {
        cur_unique_count = 0.0;
        for (int j = 0; j < max_resources; ++j) {
            if (unique_found[i][j])
                cur_unique_count++;
        }
        // Max value is 1
        unique_per_player += (double)cur_unique_count/max_resources;
    }

    // Max here is 1
    return unique_per_player/no_players;
}

double Fitness::strategic_resource(Map& m, PlayerManager& pm) 
{
    double val(0.0);
    double total_strategics(0.0);
    vector<int> strategic_count(pm.get_no_of_players(), 0);
    for (int i = 0; i < Map::_DIMY; ++i) {
        for (int j = 0; j < Map::_DIMX; ++j) {
            if (m.at(j,i)->get_resource().get_type() == Resource::Strategic) {
                if (m.at(j,i)->get_owner().get_id() == -1) {
                    cout << "Owner of a resource is not set, did you forget to call assign_resources()? " << endl;
                    return -1.0;
                } 
                strategic_count[m.at(j,i)->get_owner().get_id()]++ ;
                total_strategics++;
            }
        }
    }
    double cur_avg;
    for(int i = 0; i < pm.get_no_of_players(); ++i) 
    {
        cur_avg = ((strategic_count[i] / total_strategics)/(1.0/pm.get_no_of_players()));
        if (cur_avg > 1.0) {
            cur_avg = 1 - ((cur_avg - 1.0)/cur_avg);
        }
        val += cur_avg;
    }
    // Max value here is 1
    return val / pm.get_no_of_players();
}

double Fitness::players_in_a_continent(Map& m,  PlayerManager& pm) 
{
    AStar search;
    double val(0);
    vector<Player> players = pm.get_players();

    // Count how many players, each player can find without going through water or mountains.
    // for N players, if each player can reach (N-1) players, N^2-N is the max count 
    // so, total_legs / N(N-1)

    for (unsigned int i(0); i < players.size(); ++i) 
    {
        for (unsigned int j(0); j < players.size(); ++j) {
            if (j != i) {
                search = AStar(
                    *m.at(players[i].get_start()),
                    *m.at(players[j].get_start()),
                    &m);
                search.allow_mountains(false);
                search.allow_embark(false);
                if (search.search()) 
                {
                    val++;
                }
            }
        }
        
    }
    // Max value here is 1
    return val/((pm.get_no_of_players())*(pm.get_no_of_players()-1));
}

double Fitness::total_throughput(Map& m) {
    double total(0.0);
    for (int i = 0; i < Map::_DIMY; ++i) {
        for (int j = 0; j < Map::_DIMX; ++j) {
            total += m.at(j,i)->get_yield().get_sum();
        }
    }
    return total;
}