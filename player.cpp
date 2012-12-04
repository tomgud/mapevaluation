#include "player.h"
#include "map.h"

Player::Player() : id(-1), team(-1), start_pos() {}
Player::Player(int id, int team, int x, int y) : 
	id(id), team(team), start_pos(x,y) { 
}

Player::Player(int id, int team, CoOrd c) : 
	id(id), team(team), start_pos(c) { 
}

void Player::set_start(int x, int y) { this->start_pos = CoOrd(x,y); }
void Player::set_start(CoOrd c) { this->start_pos = c; }

bool Player::is_teammate(const Player& p) const { return this->team == p.get_team(); }
