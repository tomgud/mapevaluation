/*
 * CoOrd class, representing an x and y coordinates.
 * Copyright 2012 Tomas Gudmundsson
 */
#include <cmath>
#include "Map/coords.h"

CoOrd::CoOrd() : x(CoOrd::oob), y(CoOrd::oob) { }
CoOrd::CoOrd(int x, int y) : x(x), y(y) { }
CoOrd::CoOrd(const CoOrd& lval) : x(lval.x), y(lval.y) { }
CoOrd::~CoOrd() {}
CoOrd& CoOrd::operator=(const CoOrd& lval) {
    this->x = lval.x;
    this->y = lval.y;
    return *this;
}
int CoOrd::get_length(const CoOrd& c1, const CoOrd& c2) {
    int xdiff(c1.get_x()-c2.get_x());
    int ydiff(c1.get_y()-c2.get_y());
    if (xdiff < 0) xdiff *= -1;
    if (ydiff < 0) ydiff *= -1;
    return xdiff+ydiff;
}

bool CoOrd::operator==(const CoOrd& c) const {
    return (this->x == c.x && this->y == c.y);
}

double CoOrd::len() const  {
    return sqrt(this->x*this->x + this->y*this->y);
}

bool CoOrd::operator<(const CoOrd& c) const {
    return this->len() < c.len();
}
