#ifndef __output_writer_h__
#define __output_writer_h__

#include "map.h"


class OutputWriter {
public:
    static void print_html(const Map &m);
    static void write_html(const Map &m, const string& s);
    static void print_xml(Map &m);
    static void print_civ5map(Map &m);
private:
    static void print_html_header();
    static void print_html_footer();
    static void print_xml_header();
    static void print_xml_footer();
    static void print_civ5map_header();
    static void print_civ5map_footer();

};

#endif


