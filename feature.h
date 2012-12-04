#ifndef __feature__
#define __feature__
#include <string>
#include "yield.h"
#include "resource.h"

using std::string;


class Feature {
public:
	enum FeatureName {
		Atoll, 
		CorroDePotosi, // singleton 
		ElDorado, // singleton 
        FloodPlains,
        Forest,
		FountainOfYouth, // singleton 
        Ice,
        Jungle,
        Krakatoa, // singleton
        Lake,
        Marsh,
        MtFuji, // singleton
        Oasis,
        OldFaithful, // singleton
        Rivers,
        RockGibraltar, // singleton
        BarringerCrater, // singleton
        GrandMesa, // singleton
        GreatBarrierReef, // singleton
		NO_FEAT,
	};
	Feature();
	Feature(FeatureName name);
	Feature(string str_name);
	FeatureName get_name() const {return this->name;};
    string get_string() const;
    Yield get_yield() const { return this->yield; }
    bool is_singleton() const { return this->singleton; }
    static bool is_singleton(FeatureName);
    bool is_impassable() const { return this->impassable; }
protected :
	void init();
	FeatureName name;
	Yield yield;
	int mv_cost;
	float combat_mod;
	bool singleton, impassable;
};

#endif
