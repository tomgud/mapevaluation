#ifndef __yield__
#define __yield__

class Yield {
private:
	int food, production, gold;
public:
	Yield();
	Yield(int food, int production, int gold);
	int get_food() const;
	int get_prod() const;
	int get_gold() const;
	int get_sum() const;
	Yield operator+(const Yield&) const;
	bool operator<(const Yield&) const;
};

#endif
