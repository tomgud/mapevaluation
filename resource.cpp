#include "resource.h"
#include <string>

Resource::Resource() : name(Resource::NO_RES_NAME), type(Resource::NO_RES_TYPE),
    yield(0, 0, 0) {

}

Resource::Resource(string name) : name(Resource::NO_RES_NAME),
    type(Resource::NO_RES_TYPE), yield(0, 0, 0)
{
	if (name.find("RESOURCE") != string::npos) {
		name.erase(name.find("RESOURCE"), 9);
	}

	if (name == "BANANA") {
		this->name = Banana;
	} else if (name == "COW") {
		this->name = Cattle;
	} else if (name == "DEER") {
		this->name = Deer;
	} else if (name == "FISH") {
		this->name = Fish;
	} else if (name == "SHEEP") {
		this->name = Sheep;
	} else if (name == "STONE") {
		this->name = Stone;
	} else if (name == "WHEAT") {
		this->name = Wheat;
	} else if (name == "ALUMINUM") {
		this->name = Aluminum;
	} else if (name == "COAL") {
		this->name = Coal;
	} else if (name == "HORSE") {
		this->name = Horses;
	} else if (name == "IRON") {
		this->name = Iron;
	} else if (name == "OIL") {
		this->name = Oil;
	} else if (name == "URANIUM") {
		this->name = Uranium;
	} else if (name == "COTTON") {
		this->name = Cotton;
	} else if (name == "DYE") {
		this->name = Dyes;
	} else if (name == "FURS") {
		this->name = Furs;
	} else if (name == "GEMS") {
		this->name = Gems;
	} else if (name == "GOLD") {
		this->name = Gold;
	} else if (name == "INCENSE") {
		this->name = Incense;
	} else if (name == "IVORY") {
		this->name = Ivory;
	} else if (name == "MARBLE") {
		this->name = Marble;
	} else if (name == "PEARLS") {
		this->name = Pearls;
	} else if (name == "SILK") {
		this->name = Silk;
	} else if (name == "SILVER") {
		this->name = Silver;
	} else if (name == "SPICES") {
		this->name = Spices;
	} else if (name == "SUGAR") {
		this->name = Sugar;
	} else if (name == "WHALE") {
		this->name = Whales;
	} else if (name == "WINE") {
		this->name = Wine;
	} else {
		this->name = Resource::NO_RES_NAME;
	}
	this->init(this->name);
}

Resource::Resource(ResourceName name) : name(name), type(Resource::NO_RES_TYPE),
    yield(0, 0, 0)
{
    this->init(name);
}

void Resource::init(ResourceName name) {

	if (name < 100) {
		this->type = Resource::Bonus;
		if (name == Resource::Fish) {
			this->yield = Yield(2, 0, 0);
		} else if (name == Resource::Stone) {
			this->yield = Yield(0, 1, 0);
		} else {
			this->yield = Yield(1, 0, 0);
		}
	} else if (name >= 100 && name < 200) {
		this->type = Resource::Strategic;
		this->yield = Yield(0, 1, 0);
	} else if (name < 300) {
		this->type = Resource::Luxury;
		if (name == Resource::Gems) {
			this->yield = Yield(0, 0, 3);
		} else if (name == Resource::Whales) {
			this->yield = Yield(1, 0, 1);
		} else {
			this->yield = Yield(0, 0, 2);
		}
	} else {
		this->type = Resource::NO_RES_TYPE;
		this->yield = Yield();
	}
}

string Resource::get_string() const {
	switch (this->name) {
        case Banana:
            return "Banana";
        case Cattle:
            return "Cattle";
        case Deer:
            return "Deer";
        case Fish :
            return "Fish";
        case Sheep:
            return "Sheep";
        case Stone:
            return "Stone";
        case Wheat:
            return "Wheat";
        case Aluminum:
            return "Aluminum";
        case Coal:
            return "Coal";
        case Horses:
            return "Horses";
        case Iron :
            return "Iron";
        case Oil :
            return "Oil";
        case Uranium :
            return "Uranium";
        case Cotton:
            return "Cotton";
        case Dyes:
            return "Dyes";
        case Furs:
            return "Furs";
        case Gems:
            return "Gems";
        case Gold:
            return "Gold";
        case Incense:
            return "Incense";
        case Ivory:
            return "Ivory";
        case Marble:
            return "Marble";
        case Pearls:
            return "Pearls";
        case Silk:
            return "Silk";
        case Silver:
            return "Silver";
        case Spices:
            return "Spices";
        case Sugar:
            return "Sugar";
        case Whales:
            return "Whales";
        case Wine:
            return "Wine";
        case NO_RES_NAME:
        default:
            return "";
    }
}

Resource::ResourceName Resource::get_name(unsigned int index) {
    index = index % 28; // (7 + 6 + 15 ); 
    if (index < 7 ){
        return ResourceName(index);
    } else if (index < 13) {
        return ResourceName(100+index-7);
    } else if (index < 28) {
        return ResourceName(200+index-13);
    } else {
        return ResourceName(Resource::NO_RES_NAME);
    }
}

int Resource::get_index(ResourceName name) 
{
	int value = static_cast<int>(name);
	if (value == 300)
		value = 28;
	else if (value >= 200)
		value = value - 200 + 13;
	else if (value >= 100)
		value = value - 100 + 7;
	return value;
}
