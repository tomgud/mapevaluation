/*
 * IndGrade stands for individiual grade
 * and this file contains the comparator 
 * class for it as well. It gives a single
 * genome a grade.
 *
 * Copyright 2012 Tomas Gudmundsson
 */
#include "Evolution/ind_grade.h"
#include <iostream>
IndGrade::IndGrade(unsigned int id, double fit1, double fit2, double fit3,
        double fit4, double fit5) : id(id), selected(0) {
    this->fitness[0] = fit1;
    this->fitness[1] = fit2;
    this->fitness[2] = fit3;
    this->fitness[3] = fit4;
    this->fitness[4] = fit5;
};

IndGrade& IndGrade::operator=(const IndGrade& rval) {
    this->id = rval.id;
    this->selected = rval.selected;
    this->fitness[0] = rval.fitness[1];
    this->fitness[1] = rval.fitness[2];
    this->fitness[2] = rval.fitness[3];
    this->fitness[3] = rval.fitness[4];
    this->fitness[4] = rval.fitness[5];
    return *this;
}
