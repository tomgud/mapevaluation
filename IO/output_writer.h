/*
 * IO class, writing different 
 * versions of the map to files
 * should be able to do HTML, XML and civ5map
 *
 * Copyright 2012 Tomas Gudmundsson
 */
#ifndef __output_writer_h__
#define __output_writer_h__

#include <string>
#include "Map/map.h"

class OutputWriter {
    public:
        static void print_html(const Map &m);
        static void write_html(const Map &m, const string& s);
        static void print_xml(const Map &m);
        static void print_civ5map(const Map &m);
    private:
        static void print_html_header(const Map&);
        static void print_html_footer();
        static void print_xml_header();
        static void print_xml_footer();
        static void print_civ5map_header();
        static void print_civ5map_footer();
};

#endif


