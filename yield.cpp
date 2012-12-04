#include "yield.h"

Yield::Yield() : food(0), production(0), gold(0) 
{
	
}

Yield::Yield(int food, int prod, int gold) :
	food(food), production(prod), gold(gold)
{
	
}

Yield Yield::operator+(const Yield& rval) const
{
	return Yield(this->food + rval.get_food(), 
		this->production + rval.get_prod(),
		this->gold + rval.get_gold());
}

int Yield::get_gold() const {
	return this->gold;
}
int Yield::get_food() const {
	return this->food;
}
int Yield::get_prod() const {
	return this->production;
}

int Yield::get_sum() const {
	return this->production + this->food + this->gold;
}

bool Yield::operator<(const Yield& ref) const {
	return this->get_sum() < ref.get_sum();
}