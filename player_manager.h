#ifndef __player_manager_h__
#define __player_manager_h__

#include "player.h"
#include "coords.h"
#include <vector>

class Genome;
using std::vector;

class PlayerManager {
public:
    PlayerManager();
    PlayerManager(int no_players = 2);
    PlayerManager(const Genome&);
    ~PlayerManager();
    Player get_player(int id);
    vector<Player> get_players();
    vector<Player> get_teammembers(int teamid);
    int get_no_of_players() const { return this->no_of_players; } 
    void set_coord(int, const CoOrd&);
private:
    int no_of_players;
    vector<Player> players;

};

#endif 

