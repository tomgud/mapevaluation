/*
 * IndGrade stands for individiual grade
 * and this file contains the comparator 
 * class for it as well. It gives a single
 * genome a grade.
 *
 * Copyright 2012 Tomas Gudmundsson
 */
#ifndef __ind_grade_h__
#define __ind_grade_h__
class IndGrade {
    private:
        unsigned int id, selected;
        double fitness[5];

    public:
        IndGrade(unsigned int id, double fit1, double fit2,
                double fit3, double fit4, double fit5);
        IndGrade& operator=(const IndGrade&);
        void set_selected(unsigned int id) {
            this->selected = (id < 5) ? id : this->selected;
        }
        void set_fit(const int& id, double f) {
            this->fitness[id] = f;
        }
        void add_to_fit(const int& id, double f) { this->fitness[id] += f; }
        unsigned int get_id() const { return this->id; }
        int get_selected() const { return this->selected; }
        double get_fit(const int& id) const { return this->fitness[id]; }
        double get_sum() const {
            return (fitness[0] + fitness[1] + fitness[2]
                    + fitness[3] + fitness[4]);
        }
        bool operator>(const IndGrade& ref) {
            return this->get_sum() > ref.get_sum();
        }
};

#endif

#ifndef __ind_grade_comp_h__
#define __ind_grade_comp_h__

class IndGradeComp {
    public:
        explicit IndGradeComp(const bool& rev = false) : reverse(rev) {}
        bool operator() (const IndGrade *lhs, const IndGrade *rhs) const {
            return lhs->get_fit(lhs->get_selected()) >
                rhs->get_fit(rhs->get_selected());
        }

    private:
        bool reverse;
};

#endif
