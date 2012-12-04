/*
 * Player manager class takes care of
 * players. Holds a number of players
 * and tells you if they are teammates
 *
 * Copyright 2012 Tomas Gudmundsson
 */
#ifndef __player_manager_h__
#define __player_manager_h__

#include <vector>
#include "Core/player.h"
#include "Map/coords.h"

class Genome;
using std::vector;

class PlayerManager {
    public:
        PlayerManager();
        explicit PlayerManager(int no_players = 2);
        explicit PlayerManager(const Genome&);
        ~PlayerManager();
        Player get_player(int id) const;
        vector<Player> get_players() const;
        vector<Player> get_teammembers(int teamid) const;
        int get_no_of_players() const { return this->no_of_players; }
        void set_coord(int, const CoOrd&);
    private:
        int no_of_players;
        vector<Player> players;
};
#endif

