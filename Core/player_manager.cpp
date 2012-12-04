/*
 * Player manager class takes care of
 * players. Holds a number of players
 * and tells you if they are teammates
 *
 * Copyright 2012 Tomas Gudmundsson
 */
#include "Core/player_manager.h"
#include <vector>
#include <iostream>
#include "Core/player.h"
#include "Evolution/genome.h"
#include "Map/coords.h"
#include "Map/map.h"

PlayerManager::PlayerManager()
    : no_of_players(0), players(0)  {
}

PlayerManager::~PlayerManager() {
    while (!this->players.empty())
        this->players.pop_back();
}

PlayerManager::PlayerManager(int no_players)
    : no_of_players(no_players),
    players(0) {
    // TODO(tomgud): Removed _DIMY and _DIMX because
    // these dimension are now on a per map basis
    if (no_players == 2) {
//        players.push_back(Player(0, 0, (Map::_DIMX/2), (Map::_DIMY/4)));
//        players.push_back(Player(1, 1, (Map::_DIMX/2), 3*(Map::_DIMY/4)));
    } else if (no_players == 4) {
        /*
        pl.push_back(Player(0, 0, CoOrd((Map::_DIMX/4), (Map::_DIMY/4))));
        pl.push_back(Player(1, 0, CoOrd((Map::_DIMX/4), 3*(Map::_DIMY/4))));
        pl.push_back(Player(2, 1, CoOrd(3*(Map::_DIMX/4), (Map::_DIMY/4)))); 
        pl.push_back(Player(3, 1, CoOrd(3*(Map::_DIMX/4), 3*(Map::_DIMY/4))));
        */
    }
}

PlayerManager::PlayerManager(const Genome& g)
    : no_of_players(0), players(0) {
    vector<CoOrd> p_c = g.get_player_starts();
    if (p_c.size() == 2) {
        this->no_of_players = 2;
        // Player* tmp;
        // tmp = new Player(0,0, p_c[0].get_x(), p_c[0].get_y());
        // this->players.push_back(tmp);
        // delete tmp;
        // tmp = new Player(1,1, p_c[1].get_x(), p_c[1].get_y());
        // this->players.push_back(tmp);
    }
    else if (p_c.size() == 4) {
        this->no_of_players = 4;
        this->players.push_back(Player(0, 0, p_c[0].get_x(), p_c[0].get_y()));
        this->players.push_back(Player(1, 0, p_c[1].get_x(), p_c[1].get_y()));
        this->players.push_back(Player(2, 1, p_c[2].get_x(), p_c[2].get_y()));
        this->players.push_back(Player(3, 1, p_c[3].get_x(), p_c[3].get_y()));
    } else {
        // errorz
    }
}

vector<Player> PlayerManager::get_players() const {
    return this->players;
}

vector<Player> PlayerManager::get_teammembers(int teamid) const {
    vector<Player> val;
    for (unsigned int i = 0; i < this->players.size(); ++i) {
        if (players[i].get_team() == teamid)
            val.push_back(players[i]);
    }
    return val;
}

Player PlayerManager::get_player(int id) const {
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
