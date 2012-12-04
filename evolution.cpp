#include "evolution.h"
#include "genome.h"
#include "map.h"
#include "player_manager.h"
#include "mtrand.h"
#include "ind_grade.h"
#include "fitness.h"
#include <vector>
#include <queue>
#include <ctime>
#include <cmath>
using std::priority_queue;

#include <iostream>
using std::cout;
using std::endl;

Evolution::Evolution(unsigned int seed, int iters, int sz, double min_ch, int genome_sz, int no_of_p) : 
seed(seed), iters(iters), size(sz), genome_size(genome_sz), no_of_players(no_of_p),
min_change(min_ch), all_req_min_chg(true), gen_pop(0)
{
	Genome cur_gen;
	for (int indiv(1); indiv <= this->size; ++indiv) {
		cur_gen = Genome(seed/indiv, genome_sz, no_of_players);
		gen_pop.push_back(cur_gen);
	}
}

void Evolution::run() 
{
	// Just for fun, time outputs.
	//char buffer[100];
	time_t rawtime;
	time ( &rawtime );

	double c_f1(0.0), c_f2(0.0), c_f3(0.0), c_f4(0.0), c_f5(0.0), cur_val_before(0.0), cur_val_now(0.0);
	IndGrade* c_ig;
	Map* c_map;
	PlayerManager* c_pm;
	priority_queue<IndGrade*, vector<IndGrade*>, IndGradeComp> indiv_one;
	priority_queue<IndGrade*, vector<IndGrade*>, IndGradeComp> indiv_two;
	for (int i = 0; i < iters; ++i) {
		for (int j = 0; j < this->size; ++j) { 
			c_pm = new PlayerManager(this->gen_pop[j]);
			c_map = new Map(this->gen_pop[j], *c_pm);
			Fitness::assign_resources(*c_map, *c_pm, false);
			c_f1 = Fitness::throughput_per_player(*c_map, *c_pm);
			c_f2 = Fitness::strategic_resource(*c_map, *c_pm);
			c_f3 = Fitness::players_in_a_continent(*c_map, *c_pm);
			c_f4 = Fitness::starting_positions(*c_map, *c_pm);
			c_f5 = Fitness::unique_resources(*c_map, *c_pm);
			c_ig = new IndGrade(this->gen_pop[j].get_seed(), c_f1, c_f2, c_f3, c_f4, c_f5);
			cur_val_now += c_ig->get_sum();

			indiv_one.push(c_ig);
			delete c_map;
			delete c_pm;
		}

		// First fitness function
		for (unsigned int k = 0; k < indiv_one.size(); ++k) {
			if (k < (3*indiv_one.size())/10) {
				indiv_one.pop();
			} else {
				c_ig = indiv_one.top();
				c_ig->set_selected(1);
				indiv_two.push(c_ig);
				indiv_one.pop();
			}
		}
		// Second fitness function
		for (unsigned int k = 0; k < indiv_two.size(); ++k) {
			if (k < (2*indiv_two.size())/7) {
				indiv_two.pop();
			} else {
				c_ig = indiv_two.top();
				c_ig->set_selected(2);
				indiv_one.push(c_ig);
				indiv_two.pop();
			}
		}
		// Third fitness function
		for (unsigned int k = 0; k < indiv_one.size(); ++k) {
			if (k < (indiv_one.size())/4) {
				indiv_one.pop();
			} else {
				c_ig = indiv_one.top();
				c_ig->set_selected(3);
				indiv_two.push(c_ig);
				indiv_one.pop();
			}
		}

		// Fourth fitness function
		for (unsigned int k = 0; k < indiv_two.size(); ++k) {
			if (k < (indiv_two.size())/3) {
				indiv_two.pop();
			} else {
				c_ig = indiv_two.top();
				c_ig->set_selected(4);
				indiv_one.push(c_ig);
				indiv_two.pop();
			}
		}

		vector<Genome> survivors(0);
		for (unsigned k = 0; k < indiv_one.size(); ++k) {
			c_ig = indiv_one.top();
			indiv_one.pop();
			for (int j = 0; j < this->size; ++j) {
				if (this->gen_pop[j].get_seed() == c_ig->get_id()) {
					survivors.push_back(this->gen_pop[j]);
					// cout << "Kept: [" << gen_pop[j].get_seed() << " : ";
					// cout << c_ig->get_fit(0) << " ";
					// cout << c_ig->get_fit(1) << " ";
					// cout << c_ig->get_fit(2) << " ";
					// cout << c_ig->get_fit(3) << " ";
					// cout << c_ig->get_fit(4) << "]" << endl;

					break;
				}
			}
		}
		// cout << endl << endl;

		// Creating offsprings.
		if (survivors.size() == 0) {
			// errorz
			cout << "Fatal error: For some reason, I don't have any survivors to evolve/mutate further. Quitting. Bye" << endl;
			return;
		}
		this->gen_pop.clear();
		this->gen_pop.assign(survivors.begin(), survivors.end());
		for (int j = this->gen_pop.size(); j < this->size; ++j) {
			Genome baby_cakes(survivors[i % survivors.size()]);
			baby_cakes.modify(j);
			this->gen_pop.push_back(baby_cakes);
		}

		// Stopping based on total sum of fitness evaluations.
		if (fabs(cur_val_now-cur_val_before) < min_change) {
				cout << "Stopping evolution since changes between evolutions are " << fabs(cur_val_now-cur_val_before);
				cout << " (" << cur_val_now << " - " << cur_val_before << ")" << " are less than the minimum change " << min_change << endl;
				return;
		} 
		cur_val_before = cur_val_now;
		cur_val_now = 0;

	}
}
