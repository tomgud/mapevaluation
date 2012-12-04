/*
 * Civ5MapReader reads a civ5map 
 * binary save file and stores it
 * in a Civ5Map class
 *
 * CopyRight 2012 Tomas Gudmundsson
 *
 */
#ifndef __civ5map_reader_h__
#define __civ5map_reader_h__
#include <string>
#include "Map/civ5map.h"

// This class should read a Civ5Map file and store it
// in the class Civ5Map. Used this as a reference for reading the binary file
// http://courses.cs.vt.edu/~cs2604/spring06/binio.html
class Civ5MapReader {
    public :
        typedef unsigned char byte;
        Civ5MapReader();
        explicit Civ5MapReader(string filename);
        void read();
        Civ5Map get_map() const {return this->map;}
        void report_error(int bytes_read, int pos);

    private :
        int get_int(byte *buffer, int size);
        Civ5Map map;
        string filename;
};

#endif
