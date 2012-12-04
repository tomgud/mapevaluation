#include "player_manager.h"
#include "player.h"
#include "genome.h"
#include "coords.h"
#include "map.h"
#include <vector>

#include <iostream>

PlayerManager::PlayerManager() : no_of_players(0), players(0)  {

}

PlayerManager::~PlayerManager() {
    while (!this->players.empty()) 
        this->players.pop_back();
}

PlayerManager::PlayerManager(int no_players) : no_of_players(no_players), 
    players(0)
{

    if (no_players == 2) {
        this->players.push_back(Player(0, 0, (Map::_DIMX/2), (Map::_DIMY/4)));
        this->players.push_back(Player(1, 1, (Map::_DIMX/2), 3*(Map::_DIMY/4)));
    } else if (no_players == 4) {
        this->players.push_back(Player(0, 0, CoOrd((Map::_DIMX/4), (Map::_DIMY/4))));
        this->players.push_back(Player(1, 0, CoOrd((Map::_DIMX/4), 3*(Map::_DIMY/4))));
        this->players.push_back(Player(2, 1, CoOrd(3*(Map::_DIMX/4), (Map::_DIMY/4))));	
        this->players.push_back(Player(3, 1, CoOrd(3*(Map::_DIMX/4), 3*(Map::_DIMY/4))));
   }
}

PlayerManager::PlayerManager(const Genome& g) :
    no_of_players(0), players(0)
{
    vector<CoOrd> p_c = g.get_player_starts();
    if (p_c.size() == 2) 
    {
        this->no_of_players = 2;
        // Player* tmp;
        // tmp = new Player(0,0, p_c[0].get_x(), p_c[0].get_y());
        // this->players.push_back(tmp);
        // delete tmp;
        // tmp = new Player(1,1, p_c[1].get_x(), p_c[1].get_y());
        // this->players.push_back(tmp);
    }
    else if (p_c.size() == 4) 
    {
        this->no_of_players = 4;
        this->players.push_back(Player(0,0, p_c[0].get_x(), p_c[0].get_y()));
        this->players.push_back(Player(1,0, p_c[1].get_x(), p_c[1].get_y()));
        this->players.push_back(Player(2,1, p_c[2].get_x(), p_c[2].get_y()));
        this->players.push_back(Player(3,1, p_c[3].get_x(), p_c[3].get_y()));
    }
    else
    {
        //errorz
    }
}

vector<Player> PlayerManager::get_players() {
    return this->players;
}

vector<Player> PlayerManager::get_teammembers(int teamid) 
{
    vector<Player> val;
    for (unsigned int i = 0; i < this->players.size(); ++i) 
    {
        if (players[i].get_team() == teamid)
            val.push_back(players[i]);
    }
    return val;
}

Player PlayerManager::get_player(int id) {
    if (id >= 0 && id < this->no_of_players) {
        return this->players[id];
    } else {
        return Player();
    }
}

void PlayerManager::set_coord(int id, const CoOrd& c) {
    if (id >= 0 && id < this->no_of_players) {
        this->players[id].set_start(c);
    }
}
