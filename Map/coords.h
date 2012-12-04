/*
 * CoOrd class, representing an x and y coordinates.
 * Copyright 2012 Tomas Gudmundsson
 */
#ifndef __coords_h__
#define __coords_h__
#include <sstream>
#include <string>
class CoOrd {
    /*
        This class is at least anti-symmetric, transitive and reflexive.
        #include "coords.h"
        int main() {
            CoOrd x(2,3);
            CoOrd y(3,3);
            CoOrd z(3,4);
            cout << "x < x ? " << (x<x) << endl;
            cout << "x < y ? " << (x<y) << endl;
            cout << "y < x ? " << (y<x) << endl;
            cout << "y < z ? " << (y<z) << endl;
            cout << "x < z ? " << (x<z) << endl;
            return 0;
        }
    */
    private:
        int x, y;

    public:
        static int const oob = -999;
        static int get_length(const CoOrd&, const CoOrd&);
        int get_x() const { return this->x; }
        int get_y() const { return this->y; }
        void set_x_y(int x, int y) { this->x = x; this->y = y; }
        double len() const;
        CoOrd();
        ~CoOrd();
        CoOrd(int x, int y);
        CoOrd(const CoOrd&);
        CoOrd& operator=(const CoOrd&);
        bool operator==(const CoOrd&) const;
        bool operator<(const CoOrd&) const;
        std::string to_string() const {
            std::ostringstream s;
            s << this->x << "," << this->y;
            return s.str();
        }
};

#endif
