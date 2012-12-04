#include <iostream>
#include <ctime>
#include <fstream>
#include <boost/random.hpp>
#include "map.h"
#include "genome.h"
#include "civ5mapreader.h"
#include "civ5map.h"
#include "output_writer.h"
#include "player_manager.h"
#include "fitness.h"
#include "evolution.h"
#include "ind_grade.h"

using std::cout;
using std::endl;

#include "tile_block.h"
/* To do a boost random numbers.
#include <boost/random.hpp> // Seems to work with only this header

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/exponential_distribution.hpp> <- this one doesn't work in linuxm int

    boost::mt19937 rng(seed);
    boost::variate_generator <boost::mt19937&, boost::normal_distribution<>> vg(rng, boost::normal_distribution<>(mean, std_dev));
    boost::variate_generator< boost::mt19937&, boost::exponential_distribution<> > rndm(rng, boost::exponential_distribution<>()) ;
    boost::random::uniform_int_distribution<> dist(1, 20);
    vg() is normal distribution double
    rndm() is exponential distribution double
    dist(rng) is uniform int distribution in the closed range [1,20]
*/
#include <boost/random.hpp>

void print_help() {
    cout << "Welcome to Civilization V map evolution!" << endl;
    cout << "To use this program please supply one or more of the following parameters" << endl << endl;
    cout << "   -evolve         This will run the evolution part and can be supplied with -seed" << endl;
    cout << "                   and -genome. If neither are supplied, a timestamp will be used for" << endl;
    cout << "                   the seed. -pop and -iter are also optional." << endl;
    cout << "   -evaluate-map   This will evaluate a civ5map mapfile of length 52kb (no scenario" << endl;
    cout << "                   information) when supplied with -filename. It can also be supplied" << endl;
    cout << "                   with -genome and -iter." << endl;
    cout << "   -testmutation   A run made that counts how many maps get improved with mutation and " << endl;
    cout << "                   how many get worse. Can be used with -genome and -iter." << endl;
    cout << "   -print          Will print out the map that has been evaluated through evaluate map" << endl;
    cout << "                   or the best map that was evolved." << endl;
    cout << "   -filename str   The relative path str to the civ5map file that should be used." << endl;
    cout << "   -genome str     The relative path str to the serialized file containing a genome." << endl;
    cout << "   -seed int       The seed number int to be used for random number generators." << endl;
    cout << "   -iter int       The number int of iterations to be performed on an evolution." << endl;
    cout << "   -pop int        The number int of individuals per population of an evolution." << endl << endl;
    cout << "Please select only one main method per run (testmutation, evolve or evaluate map). " << endl << endl;
    cout << "Whenever an evolution is run, the a genome file is saved under Civ5MapGenome.YYYYMMDD.genome" << endl << endl;
    cout << "Examples: " << endl;
    cout << "   ./mapeval -evolve -seed 10 -pop 40 -iter 20" << endl;
    cout << "   ./mapeval -evolve -genome stored.genome.file" << endl;
    cout << "   ./mapeval -evaluate-map -filename mapname.civ5map" << endl;
    cout << "   ./mapeval -evaluate-map -genome stored.genome.file" << endl;
    cout << "   ./mapeval -testmutation -genome stored.genome.file" << endl;

}

int main(int argc, char** argv) {
    bool evaluate(false), evolve(false), use_mapfile(false);
    bool use_genome(false), output_print(false), testmutation(false);
    unsigned int seed(time(0)), iter(50), pop(30), evaluate_iterations(10);
    unsigned int testmutate_iter(100);
    string map_filename, genome_filename;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-evaluate-map") == 0) {
            evaluate = true;
        } else if (strcmp(argv[i], "-evolve") == 0) {
            evolve = true;
        } else if (strcmp(argv[i], "-genome") == 0) {
            if (argc > (i+1)) {
                use_genome = true;
                genome_filename = argv[++i];
            } else {
                // We are at last index, and are 
                // missing a genome input file
            }
        } else if (strcmp(argv[i], "-filename") == 0) {
            if (argc > (i+1)) {
                use_mapfile = true;
                map_filename = argv[++i];
            } else {
                // We are at last index, and are 
                // missing a genome input file
            }
        } else if (strcmp(argv[i], "-print") == 0) {
            output_print = true;
        } else if (strcmp(argv[i], "-seed") == 0) {
            if (argc > (i+1)) {
                seed = atoi(argv[++i]);
            }
        } else if (strcmp(argv[i], "-testmutation") == 0) {
            testmutation = true;
        } else if (strcmp(argv[i], "-iter") == 0) {
            if (argc > (i+1)) {
                iter = atoi(argv[++i]);
            }
        } else if (strcmp(argv[i], "-pop") == 0) {
            if (argc > (i+1)) {
                pop = atoi(argv[++i]);
            }
        }
    }
    Map* print_out_map;
    PlayerManager print_out_pm(4);
    bool ready_to_print(false);


    if (evaluate && use_mapfile) {
        if (iter != 0) 
            evaluate_iterations = iter;
        cout << "Evaluating mapfile [" << map_filename << "], using seed [" << seed << "]" << endl;
        Civ5MapReader reader(map_filename);
        reader.read();
        Civ5Map civmap = reader.get_map();
        // HACK : making sure the map is initiated;
        print_out_map = new Map(*(new Genome()));
        civmap.to_map(*print_out_map);
        ready_to_print = true;
        
        // Fitness function evaluations.
        boost::mt19937 rng(seed);
        boost::random::uniform_int_distribution<> y_gen(0, Map::_DIMY-1);
        boost::random::uniform_int_distribution<> x_gen(0, Map::_DIMX-1);
        IndGrade summed_grade(0, 0, 0, 0, 0, 0);
        CoOrd cur_c;
        Tile* cur_pl_st;
        for (unsigned int i = 0; i < evaluate_iterations; ++i) {
            for (int j = 0; j < print_out_pm.get_no_of_players(); ++j) {
                // Assign a starting position for players.
                cur_c = CoOrd(x_gen(rng), y_gen(rng));
                cur_pl_st = print_out_map->at(cur_c);
                cur_pl_st = print_out_map->search_circular([&] (Tile* t) {
                    return (!t->is_impassable() && !t->is_embarkable());
                }, cur_pl_st);
                cur_c = cur_pl_st->get_coord();
                print_out_pm.set_coord(j, cur_c);
            }
            // Assign resources and sum up the grade
            Fitness::assign_resources(*print_out_map, print_out_pm, false);
            summed_grade.add_to_fit(0, Fitness::players_in_a_continent(*print_out_map, print_out_pm));
            summed_grade.add_to_fit(1, Fitness::starting_positions(*print_out_map, print_out_pm));
            summed_grade.add_to_fit(2, Fitness::unique_resources(*print_out_map, print_out_pm));
            summed_grade.add_to_fit(3, Fitness::strategic_resource(*print_out_map, print_out_pm));
            summed_grade.add_to_fit(4, Fitness::throughput_per_player(*print_out_map, print_out_pm));
            Fitness::reset_resources(*print_out_map);
        }
        cout << "Results after " << evaluate_iterations << " iterations." << endl;
        cout << "f_0; " << summed_grade.get_fit(0)/((double)evaluate_iterations) << endl;
        cout << "f_1; " << summed_grade.get_fit(1)/((double)evaluate_iterations) << endl;
        cout << "f_2; " << summed_grade.get_fit(2)/((double)evaluate_iterations) << endl;
        cout << "f_3; " << summed_grade.get_fit(3)/((double)evaluate_iterations) << endl;
        cout << "f_4; " << summed_grade.get_fit(4)/((double)evaluate_iterations) << endl;
        cout << "f_e; " << Fitness::total_throughput(*print_out_map) << endl;
    } else if (evolve && use_genome) {
        cout << "NOT WORKING: I want to evolve from a genome file" << genome_filename << endl;
    } else if (evolve) {
        cout << "Starting evolution using seed [" << seed << "] population=" << pop << ", iterations=" << iter << endl;
        // minimum change, genome size, and player number are the last four parameters for evolution
        int genome_size(40);
        Evolution e(seed, iter, pop, 0.001, genome_size, 4);
        e.run();
        vector<Genome> genomes = e.get_genomes();
        // Output of results
        std::ostringstream s2;
        s2 << "maps/map-" << seed << "-overview.html";
        std::ofstream links;
        links.open(s2.str());
        if (!links.is_open()) cout << "Failed to create overview page." << endl;
        links << "<html><head><title>Overview for maps created with seed " << seed;
        links << "</title></head><body><h2>Maps created through " << iter;
        links << " iterations with " << pop << " per population with genome size ";
        links << genome_size << ".</h2><table>";
        links << "<tr><th>Name</th><th>f0</th><th>f1</th><th>f2</th><th>f3</th>";
        links << "<th>f4</th><th>fe</th></tr>";
        double max_value = 0;
        double cur_value = 0;
        int max_index(-1);
        for (unsigned int i = 0; i < genomes.size(); ++i) {
            print_out_pm = PlayerManager(genomes[i]);
            print_out_map = new Map(genomes[i], print_out_pm);
            cur_value = Fitness::total_throughput(*print_out_map);
            max_index = (cur_value > max_value) ? i : max_index;
            max_value = (cur_value > max_value) ? cur_value : max_value;
            std::ostringstream s; 
            s << "maps/map-" << seed << "-" << i << ".html";
            OutputWriter::write_html(*print_out_map, s.str());
            Fitness::assign_resources(*print_out_map, print_out_pm, false);
            links << "<tr><td>";
            links << "<a href=\"../" << s.str() << "\">Map number " << i << "</a></td>";
            links << "<td>" << Fitness::throughput_per_player(*print_out_map, print_out_pm) << "f4</td>";
            links << "<td>" << Fitness::strategic_resource(*print_out_map, print_out_pm) << "f3</td>";
            links << "<td>" << Fitness::unique_resources(*print_out_map, print_out_pm) << "f2</td>";
            links << "<td>" << Fitness::starting_positions(*print_out_map, print_out_pm) << "f1</td>";
            links << "<td>" << Fitness::players_in_a_continent(*print_out_map, print_out_pm) << "f0</td>";
            /// Old setup
            // links << "<td>" << Fitness::players_in_a_continent(*print_out_map, print_out_pm) f0<< "</td>";
            // links << "<td>" << Fitness::starting_positions(*print_out_map, print_out_pm) f1 << "</td>";
            // links << "<td>" << Fitness::unique_resources(*print_out_map, print_out_pm) f2 << "</td>";
            // links << "<td>" << Fitness::strategic_resource(*print_out_map, print_out_pm) f3 << "</td>";
            // links << "<td>" << Fitness::throughput_per_player(*print_out_map, print_out_pm) f4 << "</td>";
            links << "<td>" << cur_value << "</td>";
            links << "</tr>" << endl;

        }
        links << "</table>";
        links << "Map with number " << max_index << " has the greatest throughput (";
        links << max_value << ").<br />" << endl;

        links.close();

    } else if (testmutation) {
        Genome parent, child;
        int improves(0), deteriates(0);
        if (iter != 0)
            testmutate_iter = iter;
        if (use_genome) {
            cout << "NOT WORKING: I will test the genome mutation based on a genome file" << genome_filename << endl;
            parent = Genome(seed, 50, 4);
        } else {
            cout << "Mutation results using seed [" << seed << "] size=50, players=4" << endl;
            parent = Genome(seed, 50, 4);
        }

        PlayerManager parent_pm = PlayerManager(parent);
        Map* parent_map = new Map(parent, parent_pm);

        Fitness::assign_resources(*(parent_map), parent_pm, false);
        IndGrade parent_grade(0, 
            Fitness::throughput_per_player(*(parent_map), parent_pm),
            Fitness::strategic_resource(*(parent_map), parent_pm),
            Fitness::unique_resources(*(parent_map), parent_pm),
            Fitness::starting_positions(*(parent_map), parent_pm),
            Fitness::players_in_a_continent(*(parent_map), parent_pm));
        for (unsigned int i = 0; i < testmutate_iter; ++i ) {
            child = Genome(parent);
            child.modify(i);
            PlayerManager child_pm(child);
            Map* child_map = new Map(child, child_pm);

            Fitness::assign_resources(*child_map, child_pm, false);
            IndGrade child_grade = IndGrade(0, 
                Fitness::throughput_per_player(*child_map, child_pm),
                Fitness::strategic_resource(*child_map, child_pm),
                Fitness::unique_resources(*child_map, child_pm),
                Fitness::starting_positions(*child_map, child_pm),
                Fitness::players_in_a_continent(*child_map, child_pm));
            // cout << parent_grade.get_sum() << " vs " << child_grade.get_sum() << endl;
            if (child_grade > parent_grade) {
                improves++;
            } else {
                deteriates++;
            }


        }
        cout << "Out of " << testmutate_iter << ", " << improves << " improved, and " << deteriates << " deteriated." << endl;
        cout << "The sum of the fitness values(5) for the parent is " << parent_grade.get_sum() << endl;
        cout << "f0; " << parent_grade.get_fit(0) << endl;
        cout << "f1; " << parent_grade.get_fit(1) << endl;
        cout << "f2; " << parent_grade.get_fit(2) << endl;
        cout << "f3; " << parent_grade.get_fit(3) << endl;
        cout << "f4; " << parent_grade.get_fit(4) << endl;
        ready_to_print = true;
    }  else {
        // print_help();
        // int_out_map = new Map(*(new Genome()));
        // TileBlock::write(*print_out_map, TileBlock::ALL_GRASS, 2,  2, 8, 8);
        // TileBlock::write(*print_out_map, TileBlock::ALL_DESERT, 12, 2, 8, 8);
        // TileBlock::write(*print_out_map, TileBlock::ALL_TUNDRA, 22, 2, 8, 8);
        // TileBlock::write(*print_out_map, TileBlock::ALL_PLAINS, 32, 2, 8, 8);
        // TileBlock::write(*print_out_map, TileBlock::ALL_MOUNTAIN, 2, 12, 8, 8);
        // TileBlock::write(*print_out_map, TileBlock::ALL_HILL, 12, 12, 8, 8);
        // TileBlock::write(*print_out_map, TileBlock::PTN_ONE, 22, 12, 8, 8);
        // TileBlock::write(*print_out_map, TileBlock::PTN_TWO, 32, 12, 8, 8);
        // TileBlock::write(*print_out_map, TileBlock::PTN_THREE, 2, 22, 8, 8);
        // TileBlock::write(*print_out_map, TileBlock::PTN_FOUR, 12, 22, 8, 8);
        // TileBlock::write(*print_out_map, TileBlock::PTN_FIVE, 22, 22, 8, 8);
        // TileBlock::write(*print_out_map, TileBlock::PTN_SIX, 32, 22, 8, 8);
        // TileBlock::write(*print_out_map, TileBlock::PTN_SEVEN, 2, 32, 8, 8);
        // TileBlock::write(*print_out_map, TileBlock::PTN_EIGHT, 12, 32, 8, 8);
        // TileBlock::write(*print_out_map, TileBlock::PTN_NINE, 22, 32, 8, 8);
        // TileBlock::write(*print_out_map, TileBlock::PTN_TEN, 32, 32, 8, 8);
        // // print_out_map->apply_rules();
        // ready_to_print = true;
    }

    if (output_print) {
        if (ready_to_print) {
            OutputWriter::print_html(*print_out_map);
        } else {
            cout << "  Error: No map ready to be printed." << endl;
        }
    }
    // Evolution e(60, 40);
    // e.run();
    // vector<Genome> gens = e.get_genomes();
    // Genome g(1345482882, 50, 4);


    // PlayerManager pm(gens[4]);
    // Map map_from_file(gens[4], pm);

    // cout << "<h2> Genome id: " << g.get_seed() << "</h2>";

    // PlayerManager pm(4);
    // string filename = "../Civ5 Resources/SkirmishRandomSmall.Civ5Map";
    // cout << "<h2>" << filename << "</h2>";
    
    /*
    Fitness::assign_resources(print_out_map, print_out_pm, false);
            cout << "Fitness all players: " << Fitness::starting_positions(print_out_map, print_out_pm) << "<br />" << endl;
            cout << "Fitness continent: " << Fitness::players_in_a_continent(print_out_map, print_out_pm) << "<br />" << endl;
            cout << "Fitness strategic resources: " << Fitness::strategic_resource(print_out_map, print_out_pm) << "<br />" << endl;
            cout << "Fitness unique resources: " << Fitness::unique_resources(print_out_map, print_out_pm) << "<br />" << endl;
            cout << "Fitness throughput per user: " << Fitness::throughput_per_player(print_out_map, print_out_pm) << "<br />" << endl;
            */
    
}
