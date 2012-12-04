#include "feature.h"
#include <iostream>
Feature::Feature() : name(Feature::NO_FEAT), 
	 yield(0, 0, 0), mv_cost(0), combat_mod(0), singleton(false), impassable(false)
{

}


Feature::Feature(string str_name) : name(NO_FEAT), yield(0, 0, 0), 
    mv_cost(0), combat_mod(0.0), singleton(false), impassable(false)
{
	if (str_name.find("FEATURE") != string::npos) {
		str_name.erase(str_name.find("FEATURE"), 8);
	}
	if (str_name == "ATOLL") {
		this->name = Atoll;
	} else if (str_name == "POTOSI") {
		this->name = CorroDePotosi;
	} else if (str_name == "EL_DORADO") {
		this->name = ElDorado;
	} else if (str_name == "FLOOD_PLAINS") {
		this->name = FloodPlains;
	} else if (str_name == "FOREST") {
		this->name = Forest;
	} else if (str_name == "FOUNTAIN_YOUTH") {
		this->name = FountainOfYouth;
	} else if (str_name == "ICE") {
		this->name = Ice;
	} else if (str_name == "JUNGLE") {
		this->name = Jungle;
	} else if (str_name == "VOLCANO") {
		this->name = Krakatoa;
	} else if (str_name == "Lakes") {
		// Lakes can not be set, they are calculated 
		// and found by algorithms
		this->name = Lake;
	} else if (str_name == "MARSH") {
		this->name = Marsh;
	} else if (str_name == "FUJI") {
		this->name = MtFuji;
	} else if (str_name == "OASIS") {
		this->name = Oasis;
	} else if (str_name == "GEYSER") {
		this->name = OldFaithful;
	} else if (str_name == "Rivers") {	
		// Rivers are not conventional features
		this->name = Rivers;
	} else if (str_name == "GIBRALTAR") {
		this->name = RockGibraltar;
	} else if (str_name == "CRATER") {
		this->name = BarringerCrater;
	} else if (str_name == "MESA") {
		this->name = GrandMesa;
	} else if (str_name == "REEF") {
		this->name = GreatBarrierReef;
	} else {
		std::cout << "Did not find: " << str_name << "<br />";
		this->name = NO_FEAT;
	}
	this->init();
}

Feature::Feature(FeatureName name) : name(name), yield(0,0,0),
    mv_cost(0), combat_mod(0.0), singleton(false), impassable(false)
{
	this->init();	
}

void Feature::init() {
	switch (this->name) {
		case Atoll:
			this->yield = Yield(1, 1, 0);
			this->mv_cost = 1;
			this->combat_mod = 0.0;
			break;
		case CorroDePotosi:
			this->yield = Yield(0, 0, 10);
			this->mv_cost = 1;
			this->combat_mod = 0.0;
			this->singleton = true;
			this->impassable = true;
			break;
		case ElDorado:
			this->yield = Yield(0, 0, 0);
			this->mv_cost = 1;
			this->combat_mod = 0.0;
			this->singleton = true;
			this->impassable = true;
			break;
        case FloodPlains :
			this->yield = Yield(2, 0, 0);
			this->mv_cost = 1;
			this->combat_mod = -0.33;
			break;
        case Forest :
			this->yield = Yield(1, 1, 0);
			this->mv_cost = 2;
			this->combat_mod = +0.25;
			break;
		case FountainOfYouth :
			this->yield = Yield(0, 0, 10);
			this->mv_cost = 1;
			this->combat_mod = 0.0;
            this->singleton = true;
			this->impassable = true;
        case Ice :
			this->yield = Yield(0, 0, 0);
			this->mv_cost = 1;
			this->combat_mod = 0.0;
			break;
        case Jungle :
			this->yield = Yield(1, -1, 0);
			this->mv_cost = 2;
			this->combat_mod = 0.25;
			break;
        case Krakatoa :
			this->yield = Yield(0, 2, 3);
			this->mv_cost = 1;
			this->combat_mod = 0.0;
			this->singleton = true;
			this->impassable = true;
			break;
        case Lake :
			this->yield = Yield(2, 1, 0);
			this->mv_cost = 0;
			this->combat_mod = 0.0;
			break;
        case Marsh :
			this->yield = Yield(-1, 0, 0);
			this->mv_cost = 2;
			this->combat_mod = -0.33;
			break;
        case MtFuji :
			this->yield = Yield(0, 2, 3);
			this->mv_cost = 1;
			this->combat_mod = 0.0;
			this->singleton = true;
			this->impassable = true;
			break;
        case Oasis :
			this->yield = Yield(3, 0, 1);
			this->mv_cost = 1;
			this->combat_mod = -0.33;
			break;
        case OldFaithful :
			this->yield = Yield(0, 2, 3);
			this->mv_cost = 1;
			this->combat_mod = 0.0;
			this->singleton = true;
			this->impassable = true;
			break;
        case Rivers :
			this->yield = Yield(0, 0, 1);
			this->mv_cost = 4;
			this->combat_mod = 0.0;
			break;
        case RockGibraltar :
			this->yield = Yield(0, 2, 3);
			this->mv_cost = 1;
			this->combat_mod = 0.0;
			this->singleton = true;
			this->impassable = true;
			break;
        case BarringerCrater :
			this->yield = Yield(0, 2, 3);
			this->mv_cost = 1;
			this->combat_mod = 0.0;
			this->singleton = true;
			this->impassable = true;
			break;
        case GrandMesa :
			this->yield = Yield(0, 2, 3);
			this->mv_cost = 1;
			this->combat_mod = 0.0;
			this->singleton = true;
			this->impassable = true;
			break;
        case GreatBarrierReef :
			this->yield = Yield(0, 2, 3);
			this->mv_cost = 1;
			this->combat_mod = 0.0;
			this->singleton = true;
			break;
		case NO_FEAT :
			this->yield = Yield(0, 0, 0);
			this->mv_cost = 0;
			this->combat_mod = 0.0;
			break;
		default: 
			break;
	}
}

string Feature::get_string() const {
    switch (this->name) {
		case Atoll:
			return "Atoll";
		case CorroDePotosi:
			return "CorroDePotosi";
		case ElDorado :
			return "ElDorado";
		case FloodPlains:
            return "FloodPlains";
		case Forest:
            return "Forest";
		case FountainOfYouth:
			return "FountainOfYouth";
		case Ice:
            return "Ice";
		case Jungle:
            return "Jungle";
		case Krakatoa:
            return "Krakatoa";
		case Lake:
            return "Lake";
		case Marsh:
            return "Marsh";
		case MtFuji:
            return "MtFuji";
		case Oasis:
            return "Oasis";
		case OldFaithful:
            return "OldFaithful";
		case Rivers:
            return "Rivers";
		case RockGibraltar:
            return "RockGibraltar";
		case BarringerCrater:
            return "BarringerCrater";
		case GrandMesa:
            return "GrandMesa";
		case GreatBarrierReef:
			return "GreatBarrierReef";
        case NO_FEAT:
        default:
	            return "";
	}
}

bool Feature::is_singleton(FeatureName fn) {
    return (fn == CorroDePotosi) || (fn == ElDorado) ||
        (fn == FountainOfYouth) || (fn == Krakatoa) ||
        (fn == MtFuji) || (fn == OldFaithful) ||
        (fn == RockGibraltar) || (fn == BarringerCrater) ||
        (fn == GrandMesa) || (fn == GreatBarrierReef); 
}
