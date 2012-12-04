#include "civ5mapreader.h"
#include "civ5map.h"
#include "desert.h"
#include "grassland.h"
#include "plains.h"
#include "tundra.h"
#include "ocean.h"
#include "snow.h"
#include "coast.h"
#include "hill.h"
#include "mountain.h"
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
using namespace std;

Civ5MapReader::Civ5MapReader() :map(), filename("") {}

Civ5MapReader::Civ5MapReader(string filename) : map(), filename(filename) {}

void Civ5MapReader::report_error(int bytes_read, int pos) {
	// Do some crazy stuff.
	cout << "FAILURE | At position :" << pos << " after reading " << bytes_read << " bytes. " << endl;
}

int Civ5MapReader::get_int(byte *buffer, int size) {
	int value = 0;
	for (int i = size-1; i >= 0; --i) {
		value = (value << 8) + buffer[i];
	}
	return value;
}

void Civ5MapReader::read() {
	// struct stat this->maps;
	 // if (stat(this->filename.c_str(), &this->maps) == 0)
        // The size of the file in bytes is in
        // cout << "size of file is : " << this->maps.st_size << endl;
    // else
        // An error occurred
		// ;
	// Setup for reading file.
	ifstream map_file (this->filename.c_str(), ios::in | ios::binary);	
	byte buffer[8];
	int i_val, width, height;
	string str_val;
	// Skipping over version type.
	map_file.seekg(1);
	
	// Reading map width
	if (!map_file.read(reinterpret_cast<char*>(buffer), 4)) { this->report_error(map_file.gcount(), map_file.tellg());}
	i_val = get_int(buffer, 4);
	this->map.set_map_width(i_val);
	// cout << map_file.tellg() << " map width should be : " << i_val << endl;
	width = i_val;
	
	// Reading map height
	if (!map_file.read(reinterpret_cast<char*>(buffer), 4)) { this->report_error(map_file.gcount(), map_file.tellg());}
	i_val = this->get_int(buffer, 4);
	this->map.set_map_height(i_val);
	// cout << map_file.tellg() << " map height should be : " << i_val << endl;
	height = i_val;
	
	// Skipping over Players scenario
	map_file.read(reinterpret_cast<char*>(buffer), 1);
	
	// Settings bitmask
	if (!map_file.read(reinterpret_cast<char*>(buffer), 4)) { this->report_error(map_file.gcount(), map_file.tellg());}
	i_val = this->get_int(buffer, 1);
	// cout << map_file.tellg() << " Misc settings bitmask ? " << i_val << endl;
	
	// Length of first feature types
	if (!map_file.read(reinterpret_cast<char*>(buffer), 4)) { this->report_error(map_file.gcount(), map_file.tellg());}
	i_val = this->get_int(buffer, 4);
	//this->map.set_len_terrain_types(i_val);
	// cout << map_file.tellg() << " Terrain types ? " << i_val << endl;
	
	// Length of second features types
	if (!map_file.read(reinterpret_cast<char*>(buffer), 4)) { this->report_error(map_file.gcount(), map_file.tellg());}
	i_val = this->get_int(buffer, 4);
	//this->map.set_len_first_feature_types(i_val);
	// cout << map_file.tellg() << " First features ? " << i_val << endl;
	
	// Length of resource types
	if (!map_file.read(reinterpret_cast<char*>(buffer), 4)) { this->report_error(map_file.gcount(), map_file.tellg());}
	i_val = this->get_int(buffer, 4);
	//this->map.set_len_second_feature_types(i_val);
	// cout << map_file.tellg() << " Resource types ? " << i_val << endl;
	
	// Unknown
	if (!map_file.read(reinterpret_cast<char*>(buffer), 4)) { this->report_error(map_file.gcount(), map_file.tellg());}
	i_val = this->get_int(buffer, 1);
	// cout << map_file.tellg() << " Unknown ? " << i_val << endl;
	
	// Length of map name
	if (!map_file.read(reinterpret_cast<char*>(buffer), 4)) { this->report_error(map_file.gcount(), map_file.tellg());}
	i_val = this->get_int(buffer, 1);
	// cout << map_file.tellg() << " Map name length ? " << i_val << endl;
	
	// Length of description
	if (!map_file.read(reinterpret_cast<char*>(buffer), 4)) { this->report_error(map_file.gcount(), map_file.tellg());}
	i_val = this->get_int(buffer, 1);
	// cout << map_file.tellg() << " Description length ? " << i_val << endl;
	
	// Skipping over some integer
	if (!map_file.read(reinterpret_cast<char*>(buffer), 4)) { this->report_error(map_file.gcount(), map_file.tellg());}
	i_val = this->get_int(buffer, 1);
	// cout << map_file.tellg() << " Unknown integer  ? " << i_val << endl;
	
	// Terrain list
	byte str_buffer[512];
	if (!map_file.read(reinterpret_cast<char*>(str_buffer), 100)) { this->report_error(map_file.gcount(), map_file.tellg());}
	str_val = "";
	for (int i = 0; i < 100; ++i) {
	
		if (str_buffer[i] != 0x00) {
			// cout << str_buffer[i];
			str_val += str_buffer[i];
		} else {
			// cout << " " << i << endl;
			this->map.add_terrain_type(str_val);
			str_val = "";
		}
	}
	
	// Feature type list
	if (!map_file.read(reinterpret_cast<char*>(str_buffer), 280)) { this->report_error(map_file.gcount(), map_file.tellg());}
	str_val = "";
	for (int i = 0; i < 280; ++i) {
		if (str_buffer[i] != 0x00) {
			// cout << str_buffer[i];
			str_val += str_buffer[i];
		} else {
			// cout << " " << i << endl;
			this->map.add_feature_type(str_val);
			str_val = "";
		}
	}
	
	// Resource list
	if (!map_file.read(reinterpret_cast<char*>(str_buffer), 417)) { this->report_error(map_file.gcount(), map_file.tellg());}
	str_val = "";
	for (int i = 0; i < 417; ++i) {
		if (str_buffer[i] != 0x00) {
			// cout << str_buffer[i];
			str_val += str_buffer[i];
		} else {
			this->map.add_resource_type(str_val);
			str_val = "";
			// cout << " " << i << endl;
		}
	}
	
	// Panning at the end of strings
	if (!map_file.read(reinterpret_cast<char*>(buffer), 2)) { this->report_error(map_file.gcount(), map_file.tellg());}
	// cout << map_file.tellg() << " Panning this->map  ? " << i_val << endl;
	
	// Length of String3 - Some map description integer
	if (!map_file.read(reinterpret_cast<char*>(buffer), 4)) { this->report_error(map_file.gcount(), map_file.tellg());}
	i_val = this->get_int(buffer, 4);
	// cout << map_file.tellg() << " String3 Map description ? " << i_val << endl;
	
	// String3 - Some map description
	// Using the newest value in i_val to determine how long it should be.
	if (!map_file.read(reinterpret_cast<char*>(str_buffer), i_val)) { this->report_error(map_file.gcount(), map_file.tellg());}
	string map_desc("");
	for (int i = 0; i < i_val; ++i) {
		map_desc += str_buffer[i];
	}
	//this->map.set_description(map_desc);
	// cout << "Map desc " << map_desc << endl;
	
	byte tile_buffer[8];
	Tile* cur_tile = new Tile();
	// Reading the map itself
	// According to world builder the coordinates go from 
	// 0,height-1 ...  width-1,height-1
	// ...             ...
	// 0,0        ...  width-1,0
    // x coords go from 0...width-1
    // y coords go from height-1...0	
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			
			if (!map_file.read(reinterpret_cast<char*>(tile_buffer), 8)) { this->report_error(map_file.gcount(), map_file.tellg());}
			if (tile_buffer[0] != 0xFF) {
				// (int)tile_buffer[0] is the tile / terrain id
				str_val = this->map.get_terrain_types((int)tile_buffer[0]);
				if (str_val == "TERRAIN_DESERT") {
					cur_tile = new Desert(j, i);
				} else if (str_val == "TERRAIN_TUNDRA") {
					cur_tile = new Tundra(j , i);
				} else if (str_val == "TERRAIN_GRASS") {
					cur_tile = new Grassland(j, i);
				} else if (str_val == "TERRAIN_PLAINS") {
					cur_tile = new Plains(j, i);
				} else if (str_val == "TERRAIN_COAST") {
					cur_tile = new Coast(j, i);	
				} else if (str_val == "TERRAIN_OCEAN") {
					cur_tile = new Ocean(j, i);	
				} else if (str_val == "TERRAIN_SNOW") {
					cur_tile = new Snow(j, i);
				} else {
					cout << "ERROR Not recognized terrain type " << str_val <<  endl;
				}
			}
			if (tile_buffer[4] != 0xFF) {
				switch (tile_buffer[4]) {
					case 0x00 :
						// Flat land 
						break;
					case 0x01 :
						cur_tile = new Hill(j, i);
						break;
					case 0x02 :
						// Mountains 
						cur_tile = new Mountain(j, i);
						break;
					default:
						break;
				}
			}
			if (tile_buffer[1] != 0xFF) {
				// Resource id
				str_val = this->map.get_resource_types((int)tile_buffer[1]);
				cur_tile->set_resource(*(new Resource(str_val)));
				// cout << j << "," << height-1-i << ": " <<  str_val << " : " << cur_tile->get_resource().get_string() << "<br />";
			} 
			if (tile_buffer[2] != 0xFF) {
				// First feature type id
				// (int)tile_buffer[2] 
				str_val = this->map.get_feature_types((int)tile_buffer[2]);	
				cur_tile->set_feature(*(new Feature(str_val)));
				// cout << j << "," << i << ": " <<  str_val << " : " << cur_tile->get_feature().get_string() << "<br />";
				
			}
			if (tile_buffer[3] != 0xFF) {
				// River.
				//cout << map_file.tellg() << "|" << j << "," << i << ": " << (int)(tile_buffer[3]) << "<br />";
			}
			// tile_buffer[5] continents
			// 0 = nothing
			// 1 Americas
			// 2 Asia
			// 3 Africa
			// 4 Europe
			if (tile_buffer[6] != 0xFF) {
				// Second feature type id 
				// But is that considered here ?
				// Were talking about volcano, gibraltar, fountain of youth etc.
				// We add 8 because there are 8 "first feature types"
				// cout << i << "," << j << ": " << this->map.get_feature_types((int)tile_buffer[6]+8) << "<br />";
				str_val = this->map.get_feature_types((int)tile_buffer[6]+8);	
				cur_tile->set_feature(*(new Feature(str_val)));
				// cout << j << "," << i << ": " <<  str_val << " : " << cur_tile->get_feature().get_string() << "<br />";
			}
			// tile_buffer[7] unknown
			this->map.set_map(j, i, cur_tile);
		}
	} 
	/*
	// Read some strings that represent tech available, victories availble etc.
	for (int run_over = 0; run_over < 23; ++run_over) {
		if (!map_file.read(reinterpret_cast<char*>(str_buffer), 512)) { this->report_error(map_file.gcount(), map_file.tellg());}
	}
	if (!map_file.read(reinterpret_cast<char*>(str_buffer), 33)) { this->report_error(map_file.gcount(), map_file.tellg());}
	
	// Read a second version of the map.
	// Should be all empty unless some scenario information.	
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if (!map_file.read(reinterpret_cast<char*>(tile_buffer), 8)) { this->report_error(map_file.gcount(), map_file.tellg());}
			for (int k = 0; k < 8; ++k) {
				if (tile_buffer[k] != 0xFF) {
					// k == 6 is roads
					// 1 contains a railroad, 0 a road and 0xFF is empty
					// k == 5 contains improvements
					// 0 is city ruins, 1 is encampment, 2 is ancient ruins
					// 3 is farm, 4, is mine, 5 is quarry, 6 is trading post,
					// 7 is lumber mill, 8 is Pasture, 9 is fishing boats
					// 10 is plantation, 11 is camp, 12 is  Oil Well,
					// 13 is offshore platform, 14 fort, 15 landmark
					// 16 academy, 17 customs house
					// 18 manufactory, 19 citadel
					
					cout << j << "," << i <<  ": Found scenario data at index (" << k << ")" << (int)tile_buffer[k] << "<br />"; 
				}
			}
		}
	}
	*/
//	cout << "Finished reading" << endl;
}

