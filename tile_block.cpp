#include "tile_block.h"
#include "map.h"
#include "ocean.h"
#include "grassland.h"
#include "snow.h"
#include "plains.h"
#include "coast.h"
#include "hill.h"
#include "plains.h"
#include "tundra.h"
#include "desert.h"
#include "mountain.h"
#include <iostream>


void TileBlock::write(Map& m, TileBlock::Patterns pat, int x, int y, int height, int width) {
    switch (pat) {
        case ALL_GRASS:
            m.loop_through_limit([&] (Tile* t) {
                t = new Grassland(t->get_x(), t->get_y()); 
                return t;
            }, x, y, width, height);
            break;
        case ALL_DESERT:
            m.loop_through_limit([&] (Tile* t) {
                t = new Desert(t->get_x(), t->get_y()); 
                return t;
            }, x, y, width, height);
            break;
        case ALL_TUNDRA:
            m.loop_through_limit([&] (Tile* t) {
                t = new Tundra(t->get_x(), t->get_y()); 
                return t;
            }, x, y, width, height);
            break;
        case ALL_MOUNTAIN:
            m.loop_through_limit([&] (Tile* t) {
                t = new Mountain(t->get_x(), t->get_y()); 
                return t;
            }, x, y, width, height);
            break;
        // case ALL_COAST:
        //     m.loop_through_limit([&] (Tile* t) {
        //         t = new Coast(t->get_x(), t->get_y()); 
        //         return t;
        //     }, x, y, width, height);
        //     break;
        case ALL_HILL:
            m.loop_through_limit([&] (Tile* t) {
                t = new Hill(t->get_x(), t->get_y()); 
                return t;
            }, x, y, width, height);
            break;
        case ALL_PLAINS:
            m.loop_through_limit([&] (Tile* t) {
                t = new Plains(t->get_x(), t->get_y()); 
                return t;
            }, x, y, width, height);
            break;
        // case ALL_SNOW:
        //     m.loop_through_limit([] (Tile* t) {
        //         t = new Snow(t->get_x(), t->get_y()); 
        //         return t;
        //     }, x, y, width, height);
        //     break;
        case PTN_ONE:
            m.loop_through_limit([&] (Tile* t) {
                int cur_x = t->get_x();
                int cur_y = t->get_y();
                if (cur_y < (y+height/4) || cur_y >= (y+(height*3)/4)) {
                    // Top quarter rows and bottom quarter rows
                    t = new Plains(cur_x, cur_y);
                } else {
                    t = new Tundra(cur_x, cur_y);
                } 
                return t;
            }, x, y, width, height);
            break;
        case PTN_TWO:
            m.loop_through_limit([&] (Tile* t) {
                int cur_x = t->get_x();
                int cur_y = t->get_y();
                if (cur_y < (y + height/2)) {
                    if (cur_x < (x + width/2)) {
                        t = new Grassland(cur_x, cur_y);
                    } else {
                        t = new Hill(cur_x, cur_y);
                    }
                } else {
                    if (cur_x < (x + width/2)) {
                        t = new Hill(cur_x, cur_y);
                    } else {
                        //t = new Grassland(cur_x, cur_y);
                    }
                }
                return t;
            }, x, y, width, height);
            break;
        case PTN_THREE:
            m.loop_through_limit([&] (Tile* t) {
                int cur_x = t->get_x();
                int cur_y = t->get_y();
                if (cur_y != (y + height - 2 ) && cur_y != (y + 1)) {
                    t = new Grassland(cur_x, cur_y);
                } else {
                    t = new Mountain(cur_x, cur_y);
                }
                return t;
            }, x, y, width, height);
            break;
        case PTN_FOUR:
            m.loop_through_limit([&] (Tile* t) {
                int cur_x = t->get_x();
                int cur_y = t->get_y();
                if (cur_y == (y) || cur_y == (y + height - 1)) {
                    t = new Coast(cur_x, cur_y);
                } else if (cur_y == (y + 1) || cur_y == (y + height - 2 )) {
                    t = new Snow(cur_x, cur_y);
                } else {
                    t = new Tundra(cur_x, cur_y);
                }
                return t;
            }, x, y, width, height);
            break;
        case PTN_FIVE:
            m.loop_through_limit([&] (Tile* t) {
                int cur_x = t->get_x();
                int cur_y = t->get_y();
                if ((cur_y % 3 == 0 && cur_x % 3 == 0) ||
                    (cur_y % 3 == 1 && cur_x % 3 == 2) ||
                    (cur_y % 3 == 2 && cur_x % 3 == 1)) {
                    t = new Grassland(cur_x, cur_y);
                } else if ((cur_y % 3 == 0 && cur_x % 3 == 1) ||
                    (cur_y % 3 == 1 && cur_x % 3 == 0) ||
                    (cur_y % 3 == 2 && cur_x % 3 == 2)) {
                    t = new Plains(cur_x, cur_y);
                } else { 
                    /* 
                    // Should be the same as else
                    if ((cur_y % 3 == 0 && cur_x % 3 == 2) ||
                       (cur_y % 3 == 1 && cur_x % 3 == 1) ||
                       (cur_y % 3 == 2 && cur_x % 3 == 0)) {}
                    */
                    t = new Hill(cur_x, cur_y);
                }
                return t;
            }, x, y, width, height);
            break;
        case PTN_SIX:
            m.loop_through_limit([&] (Tile* t) {
                int cur_x = t->get_x();
                int cur_y = t->get_y();
                if ((cur_y % 3 == 0 && cur_x % 3 == 0) ||
                    (cur_y % 3 == 1 && cur_x % 3 == 2) ||
                    (cur_y % 3 == 2 && cur_x % 3 == 1)) {
                    t = new Desert(cur_x, cur_y);
                } else if ((cur_y % 3 == 0 && cur_x % 3 == 1) ||
                    (cur_y % 3 == 1 && cur_x % 3 == 0) ||
                    (cur_y % 3 == 2 && cur_x % 3 == 2)) {
                    t = new Hill(cur_x, cur_y);
                } else { 
                    /* 
                    // Should be the same as else
                    if ((cur_y % 3 == 0 && cur_x % 3 == 2) ||
                       (cur_y % 3 == 1 && cur_x % 3 == 1) ||
                       (cur_y % 3 == 2 && cur_x % 3 == 0)) {}
                    */
                    t = new Tundra(cur_x, cur_y);
                }
                return t;
            }, x, y, width, height);
            break;
        case PTN_SEVEN:
            m.loop_through_limit([&] (Tile* t) {
                int cur_x = t->get_x();
                int cur_y = t->get_y();
                if (cur_y <= (y+height/5)) {
                    t = new Mountain(cur_x, cur_y);
                } else {
                    t = new Hill(cur_x, cur_y);
                }
                return t;
            }, x, y, width, height);
            break;
        case PTN_EIGHT: 
            m.loop_through_limit([&] (Tile* t) {
                int cur_x = t->get_x();
                int cur_y = t->get_y();
                if (cur_x < (x + (width)/4) ) {
                    t = new Desert(cur_x, cur_y);
                } else if (cur_x < (x + (3*width)/4)) {
                    t = new Plains(cur_x, cur_y);
                } else {
                    t = new Tundra(cur_x, cur_y);
                }
                return t;
            }, x, y, width, height);
            break;
        case PTN_NINE:
            m.loop_through_limit([&] (Tile* t) {
                int cur_x = t->get_x();
                int cur_y = t->get_y();
                int margin =  (height+width)/6;
                if ((cur_y <= (y -1 + margin) && (cur_x <= (x - 1 + margin) || cur_x >= (x + (width-margin)))) ||
                    (cur_y >= (y + (height-margin)) && (cur_x <= (x - 1 + margin) || cur_x >= (x + (width-margin))))) {
                    t = new Mountain(cur_x, cur_y);
                } else {
                    t = new Desert(cur_x, cur_y);
                }
                return t;
            }, x, y, width, height);
            break;
        case PTN_TEN:
            m.loop_through_limit([&] (Tile* t) {
                int cur_x = t->get_x();
                int cur_y = t->get_y();
                int margin =  (height+width)/6;
                if ((cur_y <= (y-1 + margin) && (cur_x <= (x - 1 + margin) || cur_x >= (x + (width-margin)))) ||
                    (cur_y >= (y + (height-margin)) && (cur_x <= (x -1 + margin) || cur_x >= (x + (width-margin))))) {
                    t = new Hill(cur_x, cur_y);
                } else {
                    t = new Grassland(cur_x, cur_y);
                }
                return t;
            }, x, y, width, height);
            break;
        // case ALL_OCEAN:
        case MAX_PATTERNS:
        default:
            m.loop_through_limit([&] (Tile* t) {
                t = new Grassland(t->get_x(), t->get_y()); 
                return t;
            }, x, y, width, height );
            break;
    }
}

TileBlockPos::TileBlockPos(TileBlock::Patterns pattern, CoOrd c, int height, int width) :
    pattern(pattern), c(c), height(height), width(width) {}

TileBlockPos::TileBlockPos(int pat_id, CoOrd c, int height, int width) :
    pattern(TileBlock::Patterns(pat_id%TileBlock::Patterns::MAX_PATTERNS)), c(c),
    height(height), width(width) {}
