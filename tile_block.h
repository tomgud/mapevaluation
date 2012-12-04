#ifndef __tile_block_h__
#define __tile_block_h__

#include <string>
#include "coords.h"
#include "map.h" 
class Map;
class TileBlock {
public:
    enum Patterns  {
        // ALL_OCEAN,
        ALL_GRASS,
        ALL_DESERT,
        ALL_TUNDRA,
        ALL_MOUNTAIN,
        // ALL_COAST,
        ALL_HILL,
        ALL_PLAINS,
        // ALL_SNOW,
        PTN_ONE,
        PTN_TWO,
        PTN_THREE,
        PTN_FOUR,
        PTN_FIVE,
        PTN_SIX,
        PTN_SEVEN,
        PTN_EIGHT,
        PTN_NINE,
        PTN_TEN,

        MAX_PATTERNS // INDICATOR for last value (== size of enum)
    };
    static void write(Map& m, TileBlock::Patterns = Patterns::ALL_GRASS, int x = 0, int y = 0, int height = 8, int width = 6);
private:

};

class TileBlockPos {
private:
    TileBlock::Patterns pattern;
    CoOrd c; 
    int height, width;

public:
    TileBlockPos(TileBlock::Patterns = TileBlock::Patterns::ALL_GRASS, CoOrd = CoOrd(), int = 8, int = 6); 
    TileBlockPos(int pat_id, CoOrd = CoOrd(), int = 8, int = 6);
    TileBlock::Patterns get_pat() const { return this->pattern; }
    CoOrd get_coord() const { return this->c; }
    int get_height() const { return this->height; }
    int get_width() const { return this->width; }
    void set_h_w(int he, int wi) { this->height = he; this->width = wi; }
    void add_pattern(int val) { this->pattern = TileBlock::Patterns((this->pattern + val) % TileBlock::Patterns::MAX_PATTERNS);}
};

#endif 

