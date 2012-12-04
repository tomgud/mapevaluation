#include "a_star.h"
#include "tile_comparator.h"
#include "coords.h"
#include <set>
#include <queue>
#include <vector>
#include <iostream>

AStar::AStar() : finished(false), reached_target(false), 
    embark(false), mountains(false), last_st(),
    visited_tiles(), next_tiles(), start(), end(), map(0)
{
}

AStar::AStar(Tile& start,Tile& end, Map* m) : 
    finished(false), reached_target(false),
    embark(true), mountains(false), last_st(),
    visited_tiles(), next_tiles(), 
    start(), end(), map(m) 
{
    this->start = &start;
    this->end = &end;
}

AStar::AStar(const AStar& c) : 
    finished(c.finished), reached_target(c.reached_target),
    embark(c.embark), mountains(c.mountains),
    last_st(c.last_st), visited_tiles(c.visited_tiles), 
    next_tiles(c.next_tiles), 
    start(c.start), end(c.end), map(c.map)
{
}

AStar& AStar::operator=(const AStar& rval) 
{
    this->finished = false;
    this->reached_target = false;
    this->embark = rval.embark;
    this->mountains = rval.mountains;
    this->last_st = 0;
    this->start = rval.start;
    this->end = rval.end;
    this->map = rval.map;
    this->visited_tiles.clear();
    this->next_tiles.clear();
    return *this;
}

void AStar::allow_embark(bool b) { this->embark = b; }
void AStar::allow_mountains(bool b) { this->mountains = b; }

bool AStar::search() 
{
    // Cleaning up
    this->visited_tiles.clear();
    this->next_tiles.clear();
    
    // Setting up
    SearchTile* c_st = new SearchTile(0, 0, 
            CoOrd::get_length(this->start->get_coord(),
            this->end->get_coord()), this->start->get_coord());
    this->next_tiles.push_back(c_st);
    // Making the open list a heap. Courtesy of Justin Heyes Jones
    // http://www.heyes-jones.com/astar.html
    push_heap( this->next_tiles.begin(), this->next_tiles.end(), SearchTileComparator() );
    // Starting the search.
    int count(0);
    while (!this->next_tiles.empty()) {
        count++;
        // Top of the search !
        // Pop the best node (the one with the lowest f) 
        c_st = this->next_tiles.front(); // get pointer to the node
        pop_heap( this->next_tiles.begin(), this->next_tiles.end(), SearchTileComparator() );
        this->next_tiles.pop_back();

        // Success test
        if (success_test(c_st)) {
            // succezz
            this->finished = true;
            this->reached_target = true;
            this->last_st = c_st;
            return true;
        }
        this->populate_queue(c_st);
    }
    this->last_st = c_st;
    this->finished = true;
    this->reached_target = false;
    return false;
}

void AStar::populate_queue(SearchTile* st) 
{
    vector<CoOrd> nei = this->map->at(st->get_coord())->get_all_neighbours();
    int c_hc,c_gc;
    SearchTile* c_st;
    // Looking at successors
    for (unsigned int i = 0; i < nei.size(); ++i) 
    {
        // Check if we should fly over mountains
        if (!this->mountains && 
            this->map->at(nei[i])->is_impassable()) {
            continue;
        }
        // Check if we can embark
        if (!this->embark && 
            this->map->at(nei[i])->is_embarkable()) {
            continue;
        }

        // if we reach here we can start to think about looking at this successor
        c_gc = st->get_gcost() + this->map->at(nei[i])->get_movement_cost(); 

        // First linear search of open list to find node
        typename vector<SearchTile * >::iterator open_st_result;
        for( open_st_result = this->next_tiles.begin(); open_st_result != this->next_tiles.end(); open_st_result++ )
        {
            // If we found the same state already in our next tiles (open list)
            if( (*open_st_result)->get_coord() == nei[i] )
            {
                break;                  
            }
        }
        if( open_st_result != this->next_tiles.end() )
        {
            // We found a tile in next_tiles that is the same one we are looking at
            if ((*open_st_result)->get_gcost() <= c_gc) {
                // The one on next_tiles(open list) is better, move on.
                continue;
            }
        }

        // Then linear search of closed list to find node
        typename vector<SearchTile * >::iterator closed_st_result;
        for( closed_st_result = this->visited_tiles.begin(); closed_st_result != this->visited_tiles.end(); closed_st_result++)
        {
            // If we found the same state already in our next tiles (open list)
            if( (*closed_st_result)->get_coord() == nei[i] )
            {
                break;                  
            }
        }
        if( closed_st_result != this->visited_tiles.end() )
        {
            // We found a tile in next_tiles that is the same one we are looking at
            if ((*closed_st_result)->get_gcost() <= c_gc) {
                // The one on next_tiles(open list) is better, move on.
                continue;
            }
        }

        // When we reach this, we know that this successor has not 
        // featured on either list or is better than those on those lists
        c_hc = CoOrd::get_length(nei[i], this->end->get_coord());
        c_st = new SearchTile(st, c_gc, c_hc, nei[i]);

        // Remove successor from visited if it was on it
        if ( closed_st_result != this->visited_tiles.end()) 
        {
            this->visited_tiles.erase(closed_st_result);
        }

        // Update old version on the next tiles
        if( open_st_result != this->next_tiles.end() )
        {      
            // Ready the heap again
            this->next_tiles.erase( open_st_result );
            make_heap( this->next_tiles.begin(), this->next_tiles.end(), SearchTileComparator() );
    
        }

        // heap now unsorted
        this->next_tiles.push_back(c_st);

        // sort back element into heap
        push_heap( this->next_tiles.begin(), this->next_tiles.end(), SearchTileComparator() );
    }
    this->visited_tiles.push_back(st);
}

bool AStar::success_test(SearchTile* st) {
    return st->get_coord() == this->end->get_coord();
}
