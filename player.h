#ifndef __player_h__
#define __player_h__

#include "coords.h"

class Player {
private:
	int id;
	int team;
	CoOrd start_pos;
public:
	Player();
	Player(int id, int team, int x, int y);
	Player(int id, int team, CoOrd c);
	int get_id() const { return this->id; }
	int get_team() const { return this->team; }
	CoOrd get_start() const { return this->start_pos; }
	void set_start(int, int);
	void set_start(CoOrd);
	bool is_teammate(const Player& p) const;
};

#endif

