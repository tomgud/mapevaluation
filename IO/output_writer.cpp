/*
 * IO class, writing different 
 * versions of the map to files
 * should be able to do HTML, XML and civ5map
 *
 * Copyright 2012 Tomas Gudmundsson
 */
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include "IO/output_writer.h"
#include "Map/map.h"


using std::cout;
using std::endl;

void OutputWriter::print_html_header(const Map& m) {
    cout << "<html><head><title>Randomly generated Civ5 Map</title>";
    cout << "<link rel=\"stylesheet\" href=\"style.css\" />" << endl;
    cout << "</head><body>" << endl;
    cout << "<div class=\"container\" style=\"padding:20px; width:";
    cout << m.get_dimx() * 35 << "px\">" << endl;
}

void OutputWriter::print_html(const Map &m) {
    OutputWriter::print_html_header(m);
    m.loop_through([] (std::shared_ptr<Tile> t) {
        cout << "<div class=\"hex ";
        if (t.get()->get_y() % 2 == 1)
            cout << "even ";
        else
            cout << "odd ";
        cout << t.get()->print_html();
        cout << "\">";
        cout << t.get()->get_x() << "," << t->get_y() << "</div>" << endl;
    });
    OutputWriter::print_html_footer();
}

void OutputWriter::write_html(const Map& m, const string& s) {
    std::ofstream outfile;
    outfile.open(s);
    if (!outfile.is_open())  {
        cout << "Error opening file " << s << endl;
        return;
    }

    outfile << "<html><head><title>Randomly generated Civ5 Map</title>";
    outfile << "<link rel=\"stylesheet\" href=\"style.css\" />" << endl;
    outfile << "</head><body>" << endl;
    outfile << "<div class=\"container\" style=\"padding:20px; width:";
    cout << m.get_dimx() * 35 << "px\">" << endl;
    m.loop_through([&] (std::shared_ptr<Tile> t) {
        outfile << "<div class=\"hex ";
        if (t.get()->get_y() % 2 == 1)
            outfile << "even ";
        else
            outfile << "odd ";
        outfile << t.get()->print_html();
        outfile << "\">";
        outfile << t.get()->get_x() << "," << t.get()->get_y();
        outfile << "</div>" << endl;
    });
    outfile << "</div></body></html>";
    outfile.close();
    return;
}

void OutputWriter::print_html_footer() {
    cout << "</div></body></html>";
}
